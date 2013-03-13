#ifndef UI_DIALOG_DIALOGINPUT_H
#define UI_DIALOG_DIALOGINPUT_H

class DialogInput {
	virtual void Elements(vector<Element*>& e) = 0;

	virtual int W() const = 0;
	virtual int H() const = 0;
	virtual int ProcessEvents() const = 0;
};

#endif
