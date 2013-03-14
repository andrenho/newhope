#include "defines.h"

#include <GL/glfw.h>

UI::UI()
	: RelX(0), RelY(0), active_(true), time_(0), win_w_(0), win_h_(0),
	  zoom_(2), dialog_(nullptr)
{ 
}


void
UI::Initialize()
{
	// init GLFW
	if(!glfwInit()) {
		throw ui_error("error initializing glfw");
	}
	if(!glfwOpenWindow(0, 0, 0, 0, 0, 0, 0, 0, GLFW_WINDOW)) {
		throw ui_error("error opening window");
	}
	glfwSetWindowTitle("New Hope " VERSION);
	glfwSwapInterval(1);

	// init OpenGL
	imageset_ = new class Imageset();
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// initialize layers
	layers_.push_back(new LayerTerrain(t_GRASS));
	layers_.push_back(new LayerTerrain(t_WATER));
	AddSpecificLayers();
	layers_.push_back(new LayerDialog(&dialog_));
}


UI::~UI()
{
	for(auto& layer: layers_) {
		delete layer;
	}
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
	ProcessBasicInputs();
	ProcessSpecificInputs();
}


void 
UI::ProcessBasicInputs() 
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

	// zoom
	if(glfwGetKey('=')) {
		zoom_ *= 2.0f;
		WindowResize(0, 0);
		Render();
		glfwSleep(0.2);
		return;
	} else if(glfwGetKey('-')) {
		zoom_ /= 2.0f;
		WindowResize(0, 0);
		Render();
		glfwSleep(0.2);
		return;
	}
}


void 
UI::Render() 
{
	Scene().Initialize();

	for(auto const& layer: layers_) {
		layer->Render();
	}

	Scene().Render();
}


void
UI::StartFrame()
{
	time_ = glfwGetTime();
}


void 
UI::WaitNextFrame() 
{
	if(glfwGetTime() > time_ + 1/60.0f) {
		cout << "Frame delayed!" << endl;
	}
	while(glfwGetTime() < time_ + 1/60.0f) {
		glfwSleep(0.02);
	}
}


void
UI::WindowResize(int w, int h)
{
	cout << "Resize: " << w << "x" << h << endl;
	if(w != 0 && h != 0) {
		win_w_ = w;
		win_h_ = h;
	}

	glViewport(0, 0, (GLsizei)win_w_, (GLsizei)win_h_);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, win_w_/(zoom_*16.0), 0.0f, win_h_/(zoom_*16.0));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


string
UI::Dialog(class Dialog* d)
{
	string s;
	dialog_ = d;
	while(1) {
		StartFrame();
		ProcessBasicInputs();
		if(!d->ProcessEvents()) {
			break;
		}
		Render();
		WaitNextFrame();
	}
	s = dialog_->Reply();
	delete dialog_;
	dialog_ = nullptr;
	return s;
}
