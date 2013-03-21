#ifndef UI_UI_H
#define UI_UI_H

struct ui_error : public runtime_error {
	ui_error(string s) : runtime_error(s) { }
};


class UI {
public:
	virtual ~UI();
	void Initialize();
	bool Active();
	void ProcessInputs();
	void Render();
	void StartFrame();
	void WaitNextFrame();

	string Dialog(class Dialog* d);

	inline class Imageset const& Imageset() const { return *imageset_; }
	inline int WindowW() const { return win_w_; }
	inline int WindowH() const { return win_h_; }
	inline float WindowZoomW() const { return win_w_/float(zoom_); }
	inline float WindowZoomH() const { return win_h_/float(zoom_); }
	inline int Zoom() const { return zoom_; }
	inline class Scene const& Scene() { return scene_; }

	template <typename T> Point<T> TranslateTile(int x, int y) {
		return Point<T>(x / (16 * Zoom()) - RelX, y / (16 * Zoom()) + RelY);
	}

	float RelX, RelY;

protected:
	UI();
	virtual void ProcessBasicInputs();
	virtual void ProcessSpecificInputs() = 0;
	virtual void AddSpecificLayers() = 0;

	std::vector<Layer*> layers_;
	class Scene scene_;

private:
	void WindowResize(int w, int h);

	bool active_;
	double time_;
	int win_w_, win_h_;
	float zoom_;
	class Imageset const* imageset_;
	class Dialog* dialog_;
};

#endif
