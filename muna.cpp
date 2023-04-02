
#include <glad/glad.h>
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

int width = 0, height = 0, ratio = 0;

class Shader {
	
public:
	GLuint create(GLenum type)
	{
		GLuint Shader = glCreateShader(type);

		return Shader;
	};

	void source(GLuint Shader, const char *ShaderFilePath)
	{
		string ShaderCode = "";
		
		ifstream ShaderFile(ShaderFilePath,ifstream::in);
		
		stringstream ShaderStream;

		ShaderStream << ShaderFile.rdbuf();
		
		ShaderCode = ShaderStream.str();

		const char* ShaderCodeString = ShaderCode.c_str();

		glShaderSource(Shader, 1, &ShaderCodeString, NULL);
	


	}
	void compile(GLuint Shader) {
		glCompileShader(Shader);
	};

	void log(GLuint Shader)
	{
		GLint Success;
		char InfoLog[512];
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
		if (!Success)
		{
			glGetShaderInfoLog(Shader, 512, NULL, InfoLog);
			cout << "ERROR::SHADER::COMPILATION_FAILED" << InfoLog <<  endl;;
		};

	}

	unsigned int attach(GLuint VertexShader, GLuint FragmentShader)
	{
		GLint Success;
		char InfoLog[512];
		unsigned int ShaderProgram = glCreateProgram();
		glAttachShader(ShaderProgram, VertexShader);
		glAttachShader(ShaderProgram, FragmentShader);

		glLinkProgram(ShaderProgram);

		glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
		if (!Success)
		{
			glGetProgramInfoLog(ShaderProgram, 512, NULL, InfoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLog << std::endl;
		}

		return ShaderProgram;
	}

};


class Game {
		


public: 
		GLuint VBO = 0, VAO = 0, EBO = 0;
		Shader shader;
		GLuint VertexShader = 0, FragmentShader = 0;
		unsigned int ShaderProgram = 0;
		vector<float> vertices = { 0.0 };
		vector<unsigned int> indices = { 0 };
		unsigned int texture = 0;

	void setup() {

		VertexShader = shader.create(GL_VERTEX_SHADER);
		shader.source(VertexShader, "Shader/vertex.vert");
		shader.compile(VertexShader);
		shader.log(VertexShader);

		FragmentShader = shader.create(GL_FRAGMENT_SHADER);
		shader.source(FragmentShader, "Shader/fragment.frag");
		shader.compile(FragmentShader);
		shader.log(FragmentShader);

		ShaderProgram = shader.attach(VertexShader, FragmentShader);

		float ratio = 0;
		int width = 0, height = 0;

		int img_width = 0, img_height = 0, nchannel = 0;
		unsigned char* Texture_Data = stbi_load("img/shit.png", &width, &height, &nchannel, 4);

		



		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		
		glGenTextures(1, &texture);
		glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, texture);
		// set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Texture_Data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	void update() {


		//game.vertices = {

		//	//POSITIONS             //TEXTURE CORDS
		//	0.5f,   0.5f, 0.0f,       1.0f, 1.0f, //top right // top right
		//	0.5f,  -0.5f, 0.0f,       1.0f, 0.0f,  // bottom right// bottom right
		//	-0.5f, -0.5f, 0.0f,       0.0f, 0.0f, // bottom left // bottom left
		//	-0.5f,  0.5f, 0.0f,       0.0f, 1.0f // top left  // topleft



		//};
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		//texture
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	void render() {

	}
};

int main(int argc, char argv[]) {


	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	
	window = glfwCreateWindow(640, 480, "                       Modern OpenGL - Game", NULL, NULL);


	cout << glfwGetVersionString() << endl;

	GLFWimage image[1];
	image[0].pixels = stbi_load("img/shit.png", &image[0].width, &image[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, image);


	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	

	
			//vector<float> vertices = { 0.0 };

	//vertices.size() * sizeof(GLfloat)
	

			

			
	Game game;


	game.setup();
			glUniform1i(glGetUniformLocation(game.ShaderProgram, "ourTexture"), 0);
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &width, &height);
			ratio = width / (float)height;

			glViewport(0, 0, width, height);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

	

		
			glUseProgram(game.ShaderProgram);

			//float vertices[] = {
			//	// first triangle
			//	 0.5f,  0.5f, 0.0f,  // top right
			//	 0.5f, -0.5f, 0.0f,  // bottom right
			//	-0.5f,  0.5f, 0.0f,  // top left 
			//	// second triangle
			//	 0.5f, -0.5f, 0.0f,  // bottom right
			//	-0.5f, -0.5f, 0.0f,  // bottom left
			//	-0.5f,  0.5f, 0.0f   // top left
			//};
		
			game.vertices = {

				//POSITIONS             //TEXTURE CORDS
				0.5f,   0.5f, 0.0f,       1.0f, 1.0f, //top right // top right
				0.5f,  -0.5f, 0.0f,       1.0f, 0.0f,  // bottom right// bottom right
				-0.5f, -0.5f, 0.0f,       0.0f, 0.0f, // bottom left // bottom left
				-0.5f,  0.5f, 0.0f,       0.0f, 1.0f // top left  // topleft

				
				
			};

			game.indices = {
				0, 1, 3,
				1, 2, 3
			};

			game.update();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, game.texture);
			glBindVertexArray(game.VAO);
			
		//	glDrawArrays(GL_TRIANGLES, 0, 9);
		//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, game.EBO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glfwSwapBuffers(window);


			glfwPollEvents();
		}
		
	

	glfwTerminate();

	return 0;
}