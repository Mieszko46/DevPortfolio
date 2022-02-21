#include "Application.h"

// ---------- CAMERA ----------
Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
float LastX = 800.0f / 2.0f;
float LastY = 800.0f / 2.0f;
bool FirstMouse = true;


// set up vertex data (and buffer(s)) and configure vertex attributes
float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};


Application::Application() : m_WidthWindow(1280), m_HeightWindow(960)
{
	m_DeltaFrameTime = 0.0f;
	m_LastFrameTime = 0.0f;

	m_pAnimShader = nullptr;
	m_pAnimModel = nullptr;
	m_pAnimation = nullptr;
	m_pAnimator = nullptr;
	m_pWindow = nullptr;
}

Application::~Application()
{
	FreeOpenGLProgram(); //Zwalnianie pamiêci

	glfwDestroyWindow(m_pWindow); //Usuñ kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajête przez GLFW
	exit(EXIT_SUCCESS);
}

void Application::Initialize()
{
	glfwSetErrorCallback(ErrorCallback);//Zarejestruj procedurê obs³ugi b³êdów

	if (!glfwInit()) { //Zainicjuj bibliotekê GLFW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLFW.\n");
		exit(EXIT_FAILURE);
	}

	m_pWindow = glfwCreateWindow(m_WidthWindow, m_HeightWindow, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!m_pWindow) //Je¿eli okna nie uda³o siê utworzyæ, to zamknij program
	{
		fprintf(stderr, "Nie mo¿na utworzyæ okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_pWindow); //Od tego momentu kontekst okna staje siê aktywny i polecenia OpenGL bêd¹ dotyczyæ w³aœnie jego.
	glfwSetFramebufferSizeCallback(m_pWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_pWindow, mouse_position_callback);
	glfwSetScrollCallback(m_pWindow, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekê GLEW
		fprintf(stderr, "Nie mo¿na zainicjowaæ GLEW.\n");
		exit(EXIT_FAILURE);
	}
}

void Application::Run()
{
	InitOpenGLProgram(); //Operacje inicjuj¹ce

	while (!glfwWindowShouldClose(m_pWindow)) //Tak d³ugo jak okno nie powinno zostaæ zamkniête
	{
		ProcessInput(m_pWindow, m_DeltaFrameTime);
		// per-frame time logic
		float currentFrame = glfwGetTime();
		m_DeltaFrameTime = currentFrame - m_LastFrameTime;
		m_LastFrameTime = currentFrame;
		DrawAnimation();
		glfwPollEvents(); //Wykonaj procedury callback w zaleznoœci od zdarzeñ jakie zasz³y.
	}
}

void Application::InitOpenGLProgram()
{
	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model)
	stbi_set_flip_vertically_on_load(true);

	//------- BACKGROUND --------
	glClearColor(0.05, 0.05, 0.1, 1); //Ustaw kolor czyszczenia bufora kolorów 
	glEnable(GL_DEPTH_TEST); //W³¹cz test g³êbokoœci na pikselach

	//------- LOAD MODEL & ANIMATION -------
	m_pAnimShader = new Shader("animModelShaderVS.glsl", "animModelShaderFS.glsl");
	m_pStageModel = new Model("resources/models/stage/Stage_03.obj");
	std::string path = "resources/models/skeleton/SkeletonUv.dae";
	m_pAnimModel = new Model(path);
	m_pAnimation = new Animation(path, &(*m_pAnimModel));
	m_pAnimator = new Animator(&(*m_pAnimation));
	m_pLamps = new Shader("lightCubeVS.glsl", "lightCubeFS.glsl");

	glGenVertexArrays(1, &m_CubeVAO);
	glGenBuffers(1, &m_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glBindVertexArray(m_CubeVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	glGenVertexArrays(1, &m_LampVAO);
	glBindVertexArray(m_LampVAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Application::FreeOpenGLProgram()
{
	glDeleteVertexArrays(1, &m_LampVAO);
	glDeleteBuffers(1, &m_VBO);
	delete m_pAnimShader;
	delete m_pAnimModel;
	delete m_pAnimation;
	delete m_pAnimator;
	delete m_pStageModel;
}

void Application::DrawAnimation()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// --------- LAMPS ---------
	const int LampsAmount = 3;
	glm::vec3 LampsColors[] = {
		glm::vec3(0.0, 0.5, 0.7),
		glm::vec3(0.7, 0.9, 0.0),
		glm::vec3(0.7, 0.0, 0.8)
	};

	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  0.0f,  0.0f),
	};

	// lights rotation speed
	float rotateSpeed[] = {
		0.7f,
		1.5f,
		3.0f
	};

	// point light 1
	pointLightPositions[0].x = 1.0f * sin(m_LastFrameTime * rotateSpeed[0]);
	pointLightPositions[0].y = 0.5f;
	pointLightPositions[0].z = cos(m_LastFrameTime * rotateSpeed[0]) * 1.5f;
	// point light 2
	pointLightPositions[1].x = 1.5f * sin(m_LastFrameTime * rotateSpeed[1]);
	pointLightPositions[1].y = cos(m_LastFrameTime * rotateSpeed[1]) * 1.0f + 1.5f;
	pointLightPositions[1].z = cos(m_LastFrameTime * rotateSpeed[1]);
	// point light 3
	pointLightPositions[2].x = 1.5f * sin(m_LastFrameTime * rotateSpeed[2]);
	pointLightPositions[2].y = sin(m_LastFrameTime * 0.5) * 0.8f + 1.0f;
	pointLightPositions[2].z = cos(m_LastFrameTime * rotateSpeed[2]) * 1.5f;


	// Shading
	// input
	m_pAnimator->UpdateAnimation(m_DeltaFrameTime);

	// don't forget to enable shader before setting uniforms
	m_pAnimShader->use();
	//be sure to activate shader when setting uniforms/drawing objects
	//------------ POINT LIGHTS properties --------------
	m_pAnimShader->setVec3("viewPos", glm::vec3(0.0f, 0.0f, 0.0f));
	m_pAnimShader->setPointLights(LampsColors, pointLightPositions, LampsAmount);

	// view/projection transformations
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(camera.m_Zoom), (float)m_WidthWindow / (float)m_HeightWindow, 0.1f, 100.0f);
	m_pAnimShader->setMat4("projection", projection);
	m_pAnimShader->setMat4("view", view);

	auto transforms = m_pAnimator->GetPoseTransforms();
	for (int i = 0; i < transforms.size(); ++i)
		m_pAnimShader->setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);


	// ------- render the SKELETON model ------
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
	m_pAnimShader->setMat4("model", model);
	m_pAnimModel->Draw(*m_pAnimShader);

	// ------- render the STAGE -------
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	
	m_pAnimShader->setMat4("model", model);
	m_pStageModel->Draw(*m_pAnimShader);

	// draw ---------- LAMPS ------------- objects
	m_pLamps->use();
	m_pLamps->setMat4("projection", projection);
	m_pLamps->setMat4("view", view);
	model = glm::mat4(1.0f);
	glBindVertexArray(m_LampVAO);
	for (int i = 0; i < LampsAmount; i++) {
		m_pLamps->setVec3("objectColor", LampsColors[i]);
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		m_pLamps->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glfwSwapBuffers(m_pWindow);
}

void Application::ErrorCallback(int error, const char* description)
{
	fputs(description, stderr);
}

void ProcessInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (FirstMouse)
	{
		LastX = xpos;
		LastY = ypos;
		FirstMouse = false;
	}

	float xoffset = xpos - LastX;
	float yoffset = LastY - ypos; // reversed since y-coordinates go from bottom to top

	LastX = xpos;
	LastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
