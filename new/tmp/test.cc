#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <iostream>

using namespace glm;
using namespace std;

int main()
{
	if(!glfwInit()) {
		cerr << "Failed to initialize GLFW\n" << endl;
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if(!glfwOpenWindow(200, 200, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
		cerr << "Failed to open GLFW window\n" << endl;
		glfwTerminate();
		return -1;
	}

	glewExperimental = true;
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW\n" << endl;
		return -1;
	}
	glfwSetWindowTitle("test");

	glfwEnable(GLFW_STICKY_KEYS);

	do {
		glfwSwapBuffers();
	} while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
}
