#include "defines.h"

#include <sstream>
#include <GLFW/glfw3.h>

TextWall* TextWall::staticThis = nullptr;

TextWall::TextWall(string message, int w, int h)
	: message_(message), accept_(false)
{
	w_ = w * 8;
	h_ = h * 12;

	w_ /= 16; w_ *= 16;
	h_ /= 16; h_ *= 16;

	vector<string> t;
	Wrap(message, t, w);
	if(w_ == 0 || h_ == 0) {
		h_ = t.size() * 12;
		for(string const& s: t) {
			if(s.length() * 8 > (unsigned int)w_) {
				w_ = s.length() * 8;
			}
		}
	}

	TextWall::staticThis = this;
	glfwSetKeyCallback(ui->Window(), &TextWall::KeyCallback);
}


void 
TextWall::Elements(vector<Element*>& e)
{
	vector<string> t;
	Wrap(message_, t, 0);
	int y = 0;
	for(string const& s: t) {
		e.push_back(new ElementText(s, 0, y++));
	}
}


void
TextWall::KeyCallback(GLFWwindow* w, int k, int scancode, int action, int mods)
{
	if(action == GLFW_PRESS && k == GLFW_KEY_ENTER) {
		TextWall::staticThis->accept_ = true;
	}
}


int
TextWall::ProcessEvents() const
{
	if(accept_) {
		glfwSetKeyCallback(ui->Window(), nullptr);
		return 0;
	}
	return -1;
}


void
TextWall::Wrap(string text, vector<string>& t, int cols)
{
	// TODO - actually wrap
	stringstream ss(text);
	string item;
	while(getline(ss, item, '\n')) {
		t.push_back(item);
	}
}
