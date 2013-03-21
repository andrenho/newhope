#ifndef GAME_EDITOR_MAPEDITOR_H
#define GAME_EDITOR_MAPEDITOR_H

#include "defines.h"

class MapEditor : public Map
{
public:
	MapEditor();
	~MapEditor();
	virtual enum Terrain Terrain(int x, int y) const;
	void Resize(int w, int h);

private:
	enum Terrain* tiles_;
};

#endif
