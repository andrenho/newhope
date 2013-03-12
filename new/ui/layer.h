#ifndef UI_LAYER_H
#define UI_LAYER_H

class Layer {
public:
	virtual ~Layer() { }
	virtual void Render() const = 0;

protected:
	virtual void Draw(Reference rc, int x, int y) const;
};

#endif
