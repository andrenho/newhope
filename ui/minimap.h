#ifndef UI_MINIMAP_H_
#define UI_MINIMAP_H_

class Minimap {
public:
	Minimap(int w, int h);
	virtual ~Minimap();

	void Draw(int x, int y);

protected:
	const int W, H;
};

#endif  // UI_MINIMAP_H_
