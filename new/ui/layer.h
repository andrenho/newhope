#ifndef UI_LAYER_H
#define UI_LAYER_H

class Layer {
public:
	virtual ~Layer() { }
	virtual void Render() const = 0;
};

#endif
