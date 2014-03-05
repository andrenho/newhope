// g++ test.c -std=c++11 `pkg-config --cflags --libs glfw3 glew` -o test

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>

int main()
{
	// initalize GLFW
	glewExperimental = GL_TRUE;
	glfwInit();

	// create hints for OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	
	// create window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

	// make OpenGL context active
	glfwMakeContextCurrent(window);

	// 
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// main loop
	while(!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminate GLFW
	glfwTerminate();
}
