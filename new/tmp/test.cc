#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include <iostream>

using namespace glm;
using namespace std;

int main()
{
	//
	// initialize
	//
	if(!glfwInit()) {
		cerr << "Failed to initialize GLFW\n" << endl;
		return -1;
	}

	//
	// open window
	//
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 1);

	if(!glfwOpenWindow(200, 200, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
		cerr << "Failed to open GLFW window\n" << endl;
		glfwTerminate();
		return -1;
	}

	// 
	// initialize glew
	//
	if(glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW\n" << endl;
		return -1;
	}

	glfwSetWindowTitle("test");
	glfwEnable(GLFW_STICKY_KEYS);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// 
	// create VAO
	//
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	static const GLfloat g_vertex_buffer_data[] = { 
                -1.0f, -1.0f, 0.0f,
                 1.0f, -1.0f, 0.0f,
                 0.0f,  1.0f, 0.0f,
        };
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
			g_vertex_buffer_data, GL_STATIC_DRAW);

	//
	// events
	//
	do {
		// draw
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);

		// swap buffers
		glfwSwapBuffers();
	} while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
}
