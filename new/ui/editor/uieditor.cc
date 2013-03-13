#include "defines.h"

#include <GL/glfw.h>

static const string helptext =
	"HELP\n"
	"====\n"
	"F1...This help";

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

	// new map
	if(glfwGetKey(GLFW_KEY_LCTRL) && glfwGetKey('R')) {
		Dialog(new DialogInput("Map width?", 0, 0, true));
		return;
	}

	// help
	if(glfwGetKey(GLFW_KEY_F1)) {
		Dialog(new TextWall(helptext, 30, 15));
		return;
	}
}
