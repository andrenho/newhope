#ifndef UI_DIALOG_TEXTWALL_H
#define UI_DIALOG_TEXTWALL_H

class TextWall : public Dialog {
public:
	TextWall(string message, int w=0, int h=0);
	void Elements(queue<Element>& e);
};

#endif
