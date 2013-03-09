#include "defines.h"

using namespace std;

#include <GL/glfw.h>

UI::UI()
	: active_(true), time_(0), win_w_(0), win_h_(0)
{ 
	// init GLFW
	if(!glfwInit()) {
		throw ui_error("error initializing glfw");
	}
	if(!glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		throw ui_error("error opening window");
	}
	glfwSetWindowTitle("New Hope " VERSION);

	// init OpenGL
	imageset_ = new class Imageset();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// initialize layers
	layers_.push_back(new LayerTerrain(t_GRASS));
}


UI::~UI()
{
	glfwTerminate();
}


bool 
UI::Active() 
{
	return active_; 
}


void 
UI::ProcessInputs() 
{
	// quit
	if(!glfwGetWindowParam(GLFW_OPENED) || glfwGetKey('Q')) {
		active_ = false;
		return;
	}

	// window resize
	int w, h;
	glfwGetWindowSize(&w, &h);
	if(w != win_w_ || h != win_h_) {
		WindowResize(w, h);
		return;
	}
}


void 
UI::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto const& layer: layers_) {
		layer->Render();
	}
	glfwSwapBuffers();
}


void
UI::StartFrame()
{
	time_ = glfwGetTime();
}


void 
UI::WaitNextFrame() 
{
	if(glfwGetTime() > time_ + 1/30.0f) {
		cout << "Frame delayed!" << endl;
	}
	while(glfwGetTime() < time_ + 1/30.0f) {
		glfwSleep(0.02);
	}
}


void
UI::WindowResize(int w, int h)
{
	cout << "Resize: " << w << "x" << h << endl;
	win_w_ = w;
	win_h_ = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, w/16.0, 0.0f, h/16.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
