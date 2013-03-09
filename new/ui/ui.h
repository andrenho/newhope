#ifndef UI_UI_H
#define UI_UI_H

struct glfw_error : public std::runtime_error {
	glfw_error(std::string s) : std::runtime_error(s) { }
};


class UI {
public:
	UI();
	bool Active();
	void ProcessInputs();
	void Render();
	void WaitNextFrame();

private:
	bool active_;
};

#endif
