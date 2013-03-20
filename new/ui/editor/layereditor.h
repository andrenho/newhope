#ifndef UI_EDITOR_LAYEREDITOR_H
#define UI_EDITOR_LAYEREDITOR_H

#include "defines.h"

class LayerEditor : public Layer {
public:
	LayerEditor() : Layer(), option_('T'), selected_(0) { }
	virtual void Render() const;

	int Selected() const { return selected_; }
	void setSelected(int s) { 
		// TODO
		selected_ = s; 
	}

private:
	char option_;
	int selected_;
};

#endif
