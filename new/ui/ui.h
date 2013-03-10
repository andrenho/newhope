#ifndef UI_UI_H
#define UI_UI_H

struct ui_error : public std::runtime_error {
	ui_error(std::string s) : std::runtime_error(s) { }
};


class UI {
public:
	UI();
	~UI();
	bool Active();
	void ProcessInputs();
	void Render();
	void StartFrame();
	void WaitNextFrame();

	inline class Imageset const& Imageset() const { return *imageset_; }
	inline int WindowW() const { return win_w_; }
	inline int WindowH() const { return win_h_; }

	float RelX, RelY;

private:
	void WindowResize(int w, int h);

	bool active_;
	double time_;
	int win_w_, win_h_;
	class Imageset const* imageset_;
	std::vector<Layer*> layers_;
};

#endif
