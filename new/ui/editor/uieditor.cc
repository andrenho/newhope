#include "defines.h"

#include <GL/glfw.h>
#include <cmath>

static const string helptext =
	"HELP\n"
	"====\n"
	"F1........This help\n"
	"CTRL+R....Resize map\n"
	"\n"
	"T.........Terrains\n"
	"R.........Trees\n";

UIEditor::UIEditor()
	: UI(), layer_editor_(new LayerEditor())
{
}


void 
UIEditor::ProcessSpecificInputs()
{
	// screen movement
	if(glfwGetKey(GLFW_KEY_UP)) {
		ui->RelY += 0.6;
	}
	if(glfwGetKey(GLFW_KEY_DOWN)) {
		ui->RelY -= 0.6;
	}
	if(glfwGetKey(GLFW_KEY_LEFT)) {
		ui->RelX -= 0.6;
	}
	if(glfwGetKey(GLFW_KEY_RIGHT)) {
		ui->RelX += 0.6;
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
		if(rx >= ui->WindowZoomW() - 80) { // select tile
			float bx = (rx - (ui->WindowZoomW()-80)) / 24;
			float by = ry / 24;
			if(fmod(bx, 1.0f) > .333f && fmod(by, 1.0f) > .333f) {
				SelectTile(int(bx), int(by));
			}
		} else {
			Point<int> t = ui->TranslateTile<int>(x, y);
			TileClicked(t);
		}
	}

	// options
	if(glfwGetKey('T')) {
		layer_editor_->setOption('T');
	} else if(glfwGetKey('R')) {
		layer_editor_->setOption('R');
	}
}


void
UIEditor::AddSpecificLayers()
{
	layers_.push_back(layer_editor_);
}


void 
UIEditor::SelectTile(int x, int y)
{
	layer_editor_->setSelected(x + (y * 3));
}


void 
UIEditor::TileClicked(Point<int> t)
{
	MapEditor* me = dynamic_cast<MapEditor*>(&game->Map());
	switch(layer_editor_->Option())
	{
		case 'T': // terrain
			me->SetTile(t, layer_editor_->Selected());
			break;
		case 'R': // trees
			me->AddTree(t, layer_editor_->Selected());
			break;
	}
}
