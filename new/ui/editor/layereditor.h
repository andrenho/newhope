#ifndef UI_EDITOR_LAYEREDITOR_H
#define UI_EDITOR_LAYEREDITOR_H

#include "defines.h"

class LayerEditor : public Layer {
public:
	LayerEditor() : Layer(), option_('T'), selected_(0) { }
	virtual void Render() const;

	string Selected() const;
	void setSelected(int s);

	int Option() const { return option_; }
	void setOption(char opt) { option_ = opt; }

private:
	char option_;
	int selected_;
};

#endif
