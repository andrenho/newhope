#include "defines.h"

TextWall::TextWall(string message, int w, int h)
{
	_w = w * 8;
	_h = h * 12;
}


void 
TextWall::Elements(vector<Element*>& e)
{
	e.push_back(new ElementText("Message.", 0, 0));
}
