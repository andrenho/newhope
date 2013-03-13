#include "defines.h"

void 
LayerEditor::Render() const
{
	glLoadIdentity();
	glTranslatef(0.0f, 
		float(ui->WindowH())/(ui->Zoom()*16.0f) - 1.0f, 
		0.0f);
}
