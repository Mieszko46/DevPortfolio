#pragma once
#define GLM_FORCE_RADIANS
#define GLEW_STATIC

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Model.h"
#include "Animator.h"
#include "Camera.h"
//#include "Cube.h"

const float PI = 3.14;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mod);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void ProcessInput(GLFWwindow* window, float deltaTime);

class Application 
{
public:
	// ------- METHODS -------
	Application();
	~Application();
	void Initialize();
	void Run();

private:
	// ------- VARIABLES -------
	// Window data
	unsigned int m_HeightWindow;
	unsigned int m_WidthWindow;
	GLFWwindow* m_pWindow; //Wskaünik na obiekt reprezentujπcy okno
	// Animation frame times
	float m_DeltaFrameTime;
	float m_LastFrameTime;
	// Classes pointers
	Shader* m_pAnimShader;
	Model* m_pAnimModel;
	Model* m_pStageModel;
	Animation* m_pAnimation;
	Animator* m_pAnimator;
	Shader* m_pLamps;
	// Shader data
	unsigned int m_LampVAO;
	unsigned int m_VBO;
	unsigned int m_CubeVAO;


	// ------- METHODS -------
	static void ErrorCallback(int error, const char* description);
	void InitOpenGLProgram();
	void FreeOpenGLProgram();
	unsigned int LoadTexture(char const* path);
	void DrawAnimation();
};

