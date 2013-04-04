#ifndef GAME_EDITOR_MAPEDITOR_H
#define GAME_EDITOR_MAPEDITOR_H

#include "defines.h"

class MapEditor : public Map
{
public:
	MapEditor();
	~MapEditor();
	virtual enum Terrain Terrain(Point<int> p) const;
	void Resize(int w, int h);
	void SetTile(Point<int> tile, string idx);
	TreeType Tree(Point<int> p) const;

private:
	enum Terrain* tiles_;
};

#endif
