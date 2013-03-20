#ifndef UI_UIEDITOR_H
#define UI_UIEDITOR_H

#include "defines.h"

class UIEditor : public UI {
protected:
	void AddSpecificLayers();
	void ProcessSpecificInputs();

private:
	void SelectTile(int x, int y);
};

#endif
