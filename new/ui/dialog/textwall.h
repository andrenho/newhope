#ifndef UI_DIALOG_TEXTWALL_H
#define UI_DIALOG_TEXTWALL_H

#include <GLFW/glfw3.h>

class TextWall : public Dialog {
public:
	TextWall(string message, int w=0, int h=0);
	virtual void Elements(vector<Element*>& e);
	
	int W() const { return w_; }
	int H() const { return h_; }
	virtual int ProcessEvents() const;
	virtual string Reply() const { return ""; }

protected:
	void Wrap(string text, vector<string>& t, int cols=60);

	static TextWall* staticThis;
	static void KeyCallback(GLFWwindow* w, int k, int scancode, 
			int action, int mods);

	string message_;
	int w_;
	int h_;
	bool accept_;
};

#endif
