#ifndef GAME_EDITOR_H
#define GAME_EDITOR_H

#include "defines.h"

class Editor : public Engine {
public:
	Editor() { 
		map_ = new MapEditor();
	}
};

#endif

