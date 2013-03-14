#include "defines.h"

#include <GL/glfw.h>

static const string helptext =
	"HELP\n"
	"====\n"
	"F1........This help\n"
	"CTRL+R....Resize map";

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
		int w, h;
		try {
			w = stoi(Dialog(new DialogInput("Map width?", 0, 0, true)));
			h = stoi(Dialog(new DialogInput("Map height?", 0, 0, true)));
			((MapEditor*)&game->Map())->Resize(w, h);
		} catch(invalid_argument& e) {
			Dialog(new TextWall("Invalid value."));
		}
		return;
	}

	// help
	if(glfwGetKey(GLFW_KEY_F1)) {
		Dialog(new TextWall(helptext, 30, 15));
		return;
	}

	// mouse
	if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		int x, y;
		glfwGetMousePos(&x, &y);
		float rx = x / ui->Zoom(), ry = y / ui->Zoom();
		if(rx >= ui->WindowZoomW() - 80) {
			cout << rx << "," << ry << endl;
		}
	}
}


void
UIEditor::AddSpecificLayers()
{
	layers_.push_back(new LayerEditor());
}
