#ifndef UI_DIALOG_ELEMENTS_H
#define UI_DIALOG_ELEMENTS_H

typedef enum ElementType { ELEMENT_TEXT } ElementType;

class Element { 
public:
	virtual ~Element() { }
	virtual ElementType Type() const = 0;
protected:
	Element() { }
};


class ElementText : public Element {
public:
	ElementText(string text, int x, int y) : Text(text), X(x), Y(y) { }
	ElementType Type() const { return ELEMENT_TEXT; }

	const string Text;
	const int X, Y;
};

#endif
