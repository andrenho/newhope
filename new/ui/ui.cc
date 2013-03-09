#include "defines.h"

using namespace std;

#include <GL/glfw.h>

UI::UI()
	: active_(true)
{ 
	if(!glfwInit()) {
		throw glfw_error("error initializing glfw");
	}
	if(!glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		throw glfw_error("error opening window");
	}
	glfwSetWindowTitle("New Hope " VERSION);
}

bool 
UI::Active() 
{
	return active_; 
}

void 
UI::ProcessInputs() 
{
	if(!glfwGetWindowParam(GLFW_OPENED)) {
		active_ = false;
		return;
	}
}

void 
UI::Render() 
{
	glfwSwapBuffers();
}

void 
UI::WaitNextFrame() 
{ 
}
