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
	void DrawRectangle(int x1, int y1, int x2, int y2, Color c) const;

	void WindowTileBorders(int& x, int& y, int& w, int &h) const;

private:
	mutable ProportionScene ps_;
};

#endif
