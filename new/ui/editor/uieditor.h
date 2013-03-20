#ifndef UI_UIEDITOR_H
#define UI_UIEDITOR_H

#include "defines.h"

class UIEditor : public UI {
public:
	UIEditor();

protected:
	void AddSpecificLayers();
	void ProcessSpecificInputs();

private:
	LayerEditor* layer_editor_;

	void SelectTile(int x, int y);
};

#endif
