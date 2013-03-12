#ifndef UI_DIALOG_TEXTWALL_H
#define UI_DIALOG_TEXTWALL_H

class TextWall : public Dialog {
public:
	TextWall(string message, int w=0, int h=0);
	void Elements(vector<Element*>& e);
	
	int W() const { return w_; }
	int H() const { return h_; }
	int ProcessEvents() const;

private:
	void Wrap(string text, vector<string>& t, int cols=60);

	string message_;
	int w_;
	int h_;
};

#endif
