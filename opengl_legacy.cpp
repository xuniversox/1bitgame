
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <windows.h>
void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}
using namespace std;



GLFWwindow* window;

int main(int argc, char argv[]) {


	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(640, 480, "Legacy OpenGL - Game", NULL, NULL);



	glfwMakeContextCurrent(window);
	gladLoadGL();
	
		
	
	float ratio = 0;
	int width = 0, height = 0;

	int img_width = 0, img_height = 0, nchannel = 0;
	unsigned char* Texture_Data = stbi_load("img/shit.png", &width, &height, &nchannel, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Texture_Data);
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float)height;

			glViewport(0, 0, width, height);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_QUADS);

			//BOTTOM RIGHT - RED
			glColor3f(1.0, 0.0, 0.0);
			glTexCoord2f(0.0, 0.0);
			glVertex2f(0.5f, -0.5f);

			//bottom LEFT - YELLOW
			glColor3f(1.0, 1.0, 0.0);
			glTexCoord2f(1.0, 0.0);
			glVertex2f(-0.5f, -0.5f);

			//top LEFT? - BLUE
			glColor3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(1.0, 1.0);
			glVertex2f(-0.5f, 0.5f);

			//TOP RIGHT - GREEN
			glColor3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0, 1.0);
			glVertex2f(0.5f, 0.5f);
			glEnd();
				
			glfwSwapBuffers(window);

			glfwPollEvents();
		}
		
	

	glfwTerminate();

	return 0;
}