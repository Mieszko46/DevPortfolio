#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include "string.c"

#define MAX_CLIENTS 10
#define BUFFER_SIZE 2048
#define NAME_SIZE 32

// Client structure
typedef struct clients
{
    struct sockaddr_in address;
    int sockfd;
    int uid;
    char name[NAME_SIZE];
    bool wasHost;
    int points;
} client;