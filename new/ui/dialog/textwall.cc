#include "defines.h"

#include <sstream>
#include <GL/glfw.h>

TextWall::TextWall(string message, int w, int h)
	: message_(message)
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
			if(s.length() * 8 > w_) {
				w_ = s.length() * 8;
			}
		}
	}
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


int
TextWall::ProcessEvents() const
{
	if(glfwGetKey(GLFW_KEY_SPACE) || glfwGetKey(GLFW_KEY_ENTER)
	|| glfwGetKey(GLFW_KEY_ESC)   || glfwGetMouseButton(GLFW_MOUSE_BUTTON_1)) {
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
