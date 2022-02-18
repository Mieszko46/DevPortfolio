#include "server.h"

static _Atomic unsigned int countClients = 0;
static int userId = 10;
client *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;
char hostpass[32] = "fx73host";
char wordpass[32] = "fx73pass";
char scorepass[32] = "fx73scor";
char clearpass[32] = "fx73clear";
// TODO read word from file in random line
char word[32] = "jablko";
bool bStart = true;
int hostid = 0;

void printClientAddr(struct sockaddr_in addr)
{
    printf("%d.%d.%d.%d",
        addr.sin_addr.s_addr & 0xff,
        (addr.sin_addr.s_addr & 0xff00) >> 8,
        (addr.sin_addr.s_addr & 0xff0000) >> 16,
        (addr.sin_addr.s_addr & 0xff000000) >> 24);
}

void queueAdd(client *cl)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++)
        if (!clients[i])
        {
            clients[i] = cl;
            break;
        }

    pthread_mutex_unlock(&clients_mutex);
}

void queueRemove(int uId)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++)
        if (clients[i])
            if(clients[i]->uid == uId)
            {
                clients[i] = NULL;
                break;
            }

    pthread_mutex_unlock(&clients_mutex);
}

// Send message to all clients except sender
void sendToAll(char *text, int uId)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++)
        if(clients[i])
            if(clients[i]->uid != uId) //except sender
                if(write(clients[i]->sockfd, text, strlen(text)) < 0)
                {
                    perror("ERROR: write to descriptor failed!");
                    break;
                }    
        
    pthread_mutex_unlock(&clients_mutex);
}

void sendToOne(char *text, int uId)
{
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; i++)
        if(clients[i])
            if(clients[i]->uid == uId)
                if(write(clients[i]->sockfd, text, strlen(text)) < 0)
                {
                    perror("ERROR: write to descriptor failed!");
                    break;
                }    

    pthread_mutex_unlock(&clients_mutex);
}

void sendScore()
{
    char *score = malloc(BUFFER_SIZE);
    bzero(score, BUFFER_SIZE);
    for (int i = 0; i < MAX_CLIENTS; i++)
        if(clients[i])
        {
            sprintf(score, "%s %s: %d;" , score, clients[i]->name, clients[i]->points);
        }

    char buffsend[BUFFER_SIZE];
    bzero(buffsend, BUFFER_SIZE);
    sprintf(buffsend, "%s: %s", scorepass, score);
    sendToAll(buffsend, 0);
    sleep(1);   //trzeba uzywac sleepów inaczej wiadomosci sie zlepiaja
    free(score);
}

void drawWord()
{
    bzero(word, 32);
    int filesize = 90;
    int line = rand() % filesize;

    FILE *fp = fopen("words.txt", "r");
    if (fp == NULL)
    {
        perror("Unable to open file!");
        exit(1);
    }
    
    for (int i = 0; i < line; i++)
        fgets(word, sizeof(word), fp);

    for (int i = 0; i < sizeof(word); i++)
        if((word[i] > 64 && word[i] < 91) || (word[i] > 96 && word[i] < 123))
            continue;
        else
            word[i] = 0;

    fclose(fp);
}

int getWordSize()
{
    int size = 0;
    for (int i = 0; i < sizeof(word); i++)
        if (word[i] != 0)
            size++;
    return size;
}

char * getWinner()
{
    char *winner = malloc(BUFFER_SIZE);
    bzero(winner, BUFFER_SIZE);
    int maxscore = 0;
    for (int i = 0; i < MAX_CLIENTS; i++)
        if(clients[i])
            if(clients[i]->points > maxscore)
            {
                maxscore = clients[i]->points;
                sprintf(winner, "%s" , clients[i]->name);
            }
    return winner;
}

void startGame()
{
    drawWord();
    char buffsend[BUFFER_SIZE];
    bzero(buffsend, BUFFER_SIZE);
    for (int i = 0; i < MAX_CLIENTS; i++)
        if(clients[i])
            if(!clients[i]->wasHost)
            {
                printf("--- ROUND STARTING ---\n");
                sprintf(buffsend, "\nSTARTING NEW ROUND!");
                sendToAll(buffsend, -1);
                sleep(1);

                sendScore();

                // Send word to host     
                sprintf(buffsend, "%s: %s", hostpass, word);
                sendToOne(buffsend, clients[i]->uid);
                sleep(1);  

                clients[i]->wasHost = true;
                hostid = clients[i]->uid;

                // Send word length to others
                sprintf(buffsend, "%s: %d", wordpass, getWordSize());
                sendToAll(buffsend, clients[i]->uid);
                sleep(1);
                return;
            }
    printf("--- END GAME ---\n");
    char* winner = getWinner();
    sprintf(buffsend, "\nWINNER: %s", winner);
    sendToAll(buffsend, 0);
    free(winner);
}

// Handle all comunication with the client
void *client_handler(void *arg)
{
    char buffSend[BUFFER_SIZE];
    char name[NAME_SIZE];
    bool errorflag = false;

    countClients++;
    client *ncli = (client *)arg;

    // Check inserted name
    if(recv(ncli->sockfd, name, NAME_SIZE, 0) <= 0 || strlen(name) < 2 || strlen(name) >= NAME_SIZE-1)
    {
        printf("Incorrect user name! \n");
        errorflag = true;
    } 
    else 
    {
        strcpy(ncli->name, name);
        printf("(%d)(%d)%s join to lobby.\n", ncli->uid, ncli->sockfd, ncli->name);
        sprintf(buffSend, "SERVER: %s has joined.", ncli->name);
        sendToAll(buffSend, ncli->uid);
        sleep(1);
        sendScore();

        if(!bStart)
        {
            sprintf(buffSend, "%s: %d", wordpass, getWordSize());
            sendToOne(buffSend, ncli->uid);
            sleep(0.5);
        }
    }

    bzero(buffSend, BUFFER_SIZE);

    while(1)
    {
        if(errorflag)
            break;
        
        int receive = recv(ncli->sockfd, buffSend, BUFFER_SIZE, 0);
        if (receive > 0)
        {
            if (strlen(buffSend) > 0)
            {
                if (ncli->uid != hostid && strstr(buffSend, word))
                {
                    ncli->points++;
                    printf("odgadl hasło\n");
                    sprintf(buffSend, "\nPLAYER %s GUESSED HIDDEN WORD: %s!\n", ncli->name, word);
                    sendToAll(buffSend, -1); 
                    sleep(2);   
                    sprintf(buffSend, "%s", clearpass);
                    sendToAll(buffSend, -1); 
                    sleep(1);
                    startGame();
                }
                else
                {
                    sendToAll(buffSend, ncli->uid);
                    str_trim_lf(buffSend, strlen(buffSend));
                    printf("%s\n", buffSend);
                }
            }
        }
        else if (receive == 0)
        {
            printf("(%d)(%d)%s has disconnected.\n", ncli->uid, ncli->sockfd, ncli->name);
            sprintf(buffSend, "SERVER: %s has disconnected.", ncli->name);
            sendToAll(buffSend, ncli->uid);
            errorflag = true;
            sleep(0.5);
        }
        else
        {
            printf("ERROR: -1\n");
            errorflag = true;
        }
        bzero(buffSend, BUFFER_SIZE);
    }

    // Delete client from queue and field thread
    close(ncli->sockfd);
    queueRemove(ncli->uid);
    free(ncli);
    countClients--;

    sleep(0.5);
    sendScore();

    pthread_detach(pthread_self());

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    // char *ip = "127.0.0.1";
    int port = atoi(argv[1]);
    int option = 1;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in server_addr, client_addr;
    pthread_t tid;

    // Socket setup
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Ignore pipe signals
    signal(SIGPIPE, SIG_IGN);

    if (setsockopt(listenfd, SOL_SOCKET, (SO_REUSEPORT | SO_REUSEADDR), (char*)&option, sizeof(option)) < 0)
    {
        perror("ERROR: setsockopt failed!");
        return EXIT_FAILURE;
    }

    // Bind
    if (bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("ERROR: Socket binding failed!");
        return EXIT_FAILURE;
    }

    // Listen
    if (listen(listenfd, 10) < 0)
    {
        perror("ERROR: Socket listening failed!");
        return EXIT_FAILURE;
    }

    // Print Server IP
    printf("Start Server on: %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));

    while(1)
    {
        socklen_t clilen = sizeof(client_addr);
        connfd = accept(listenfd, (struct sockaddr*)&client_addr, &clilen);

        // check if max clients is reached
        if ((countClients + 1) == MAX_CLIENTS)
        {
            printf("Clients limit reached. Rejected: ");
            printClientAddr(client_addr);
            printf(":%d\n", client_addr.sin_port);
            close(connfd);
            continue;
        }

        // Client setups
        client *cli = (client *)malloc(sizeof(client));
        cli->address = client_addr;
        cli->sockfd = connfd;
        cli->uid = userId++;
        cli->wasHost = false;
        cli->points = 0;

        // Add client to the queue and fork thread 
        queueAdd(cli);
        if (pthread_create(&tid, NULL, &client_handler, (void*)cli) != 0)
        {
            perror("ERROR: Socket listening failed");
            exit(EXIT_FAILURE);
        }

        // Reduce CPU usage
        sleep(1);

        if (bStart && countClients >= 2)
        {
            startGame();
            bStart = false;
        }   
    }
    return EXIT_SUCCESS;
}