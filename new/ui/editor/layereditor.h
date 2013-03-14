#ifndef UI_EDITOR_LAYEREDITOR_H
#define UI_EDITOR_LAYEREDITOR_H

#include "defines.h"

class LayerEditor : public Layer {
public:
	LayerEditor() : Layer(), option_('T') { }
	virtual void Render() const;

private:
	char option_;
};

#endif
