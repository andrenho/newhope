#ifndef UI_DIALOG_TEXTWALL_H
#define UI_DIALOG_TEXTWALL_H

class TextWall : public Dialog {
public:
	TextWall(string message, int w=0, int h=0);
	void Elements(vector<Element*>& e);
	
	int W() const { return _w; }
	int H() const { return _h; }

private:
	int _w;
	int _h;
};

#endif
