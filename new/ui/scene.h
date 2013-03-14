#ifndef UI_SCENE_H
#define UI_SCENE_H

typedef enum { PROP_WINDOW, PROP_TILES } ProportionScene;

class Scene {
public:
	Scene() : ps_(PROP_TILES) { }

	void Initialize() const;
	void Render() const;

	void setProportion(ProportionScene ps) const;

	void DrawImage(string index, float x, float y) const;
	void DrawRectangle(float x1, float y1, float x2, float y2, Color c) const;

	void WindowTileBorders(int& x, int& y, int& w, int &h) const;

private:
	mutable ProportionScene ps_;
};

#endif
