#ifndef UI_DIALOG_DIALOGINPUT_H
#define UI_DIALOG_DIALOGINPUT_H

#include <GLFW/glfw3.h>

#include "defines.h"

class DialogInput : public TextWall {
public:
	DialogInput(string message, int w=0, int h=0, bool numeric=false);
	virtual void Elements(vector<Element*>& e);
	virtual string Reply() const;
	virtual int ProcessEvents() const;

private:
	static void KeyCallback(GLFWwindow* w, int k, int scancode, 
			int action, int mods);
	static void CharCallback(GLFWwindow* w, unsigned int k);
	static DialogInput* staticThis;

	string reply_;
	bool numeric_;
	bool accept_;
};

#endif
