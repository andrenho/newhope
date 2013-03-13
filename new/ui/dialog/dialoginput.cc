#include "defines.h"	

#include <GL/glfw.h>

DialogInput* DialogInput::staticThis = nullptr;

DialogInput::DialogInput(string message, int w, int h, bool numeric)
	: TextWall(message, w, h), reply_(""), numeric_(numeric), accept_(false)
{ 
	h_ += 32;
	DialogInput::staticThis = this;
	glfwSetKeyCallback(&DialogInput::KeyCallback);
	glfwSetCharCallback(&DialogInput::CharCallback);
}
	

int
DialogInput::ProcessEvents() const
{
	if(accept_) {
		glfwSetKeyCallback(nullptr);
		glfwSetCharCallback(nullptr);
		return 0;
	}
	return -1;
}


void 
DialogInput::Elements(vector<Element*>& e)
{
	TextWall::Elements(e);
	e.push_back(new ElementText("", 0, e.size()));
	e.push_back(new ElementText(reply_ + 
				(int(glfwGetTime() * 2.0f) % 2 ? "_" : ""), 
				0, e.size()));
}


string 
DialogInput::Reply() const
{
	return reply_;
}


void GLFWCALL 
DialogInput::KeyCallback(int k, int action)
{
	if(action == GLFW_RELEASE) {
		return;
	}
	if(k == GLFW_KEY_BACKSPACE) {
		DialogInput::staticThis->reply_.pop_back();
	} else if(k == GLFW_KEY_ENTER) {
		DialogInput::staticThis->accept_ = true;
	}
}


void GLFWCALL 
DialogInput::CharCallback(int k, int action)
{
	if(k == '\n') {
		return;
	}
	if(DialogInput::staticThis->numeric_ && (k < '0' || k > '9')) {
		return;
	}
	DialogInput::staticThis->reply_.append(1, (char)k);
}
