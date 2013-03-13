#ifndef UI_DIALOG_DIALOGINPUT_H
#define UI_DIALOG_DIALOGINPUT_H

#include "defines.h"

class DialogInput : public TextWall {
public:
	DialogInput(string message, int w=0, int h=0, bool numeric=false);
	virtual void Elements(vector<Element*>& e);
	virtual string Reply() const;
	virtual int ProcessEvents() const;
	static DialogInput* staticThis;

private:
	static void KeyCallback(int k, int action);
	static void CharCallback(int k, int action);

	string reply_;
	bool numeric_;
	bool accept_;
};

#endif
