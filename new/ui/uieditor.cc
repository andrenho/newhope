#include "defines.h"

#include <GL/glfw.h>

void 
UIEditor::ProcessSpecificInputs()
{
	// screen movement
	if(glfwGetKey(GLFW_KEY_UP)) {
		ui->RelY += 0.3;
	}
	if(glfwGetKey(GLFW_KEY_DOWN)) {
		ui->RelY -= 0.3;
	}
	if(glfwGetKey(GLFW_KEY_LEFT)) {
		ui->RelX -= 0.3;
	}
	if(glfwGetKey(GLFW_KEY_RIGHT)) {
		ui->RelX += 0.3;
	}
}
