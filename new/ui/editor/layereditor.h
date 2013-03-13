#ifndef UI_EDITOR_LAYEREDITOR_H
#define UI_EDITOR_LAYEREDITOR_H

#include "defines.h"

class LayerEditor : public Editor {
public:
	virtual void Render() const = 0;
};

#endif
