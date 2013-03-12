#include "defines.h"

void
LayerDialog::Render() const
{
	if(!*dialog_) {
		return;
	}

	glLoadIdentity();
	glTranslatef(0.0f, 
		float(ui->WindowH())/(ui->Zoom()*16.0f) - 1.0f, 
		0.0f);

	int x, y;
	DrawBackground(x, y);
	
	vector<Element*> elements;
	(*dialog_)->Elements(elements);
	for(auto const& e: elements) {
		DrawElement(e, x, y);
		delete e;
	}
}


void
LayerDialog::DrawBackground(int& x, int& y) const
{
	Dialog* d = *dialog_;
	x = ((ui->WindowW() / 2) - (d->W() * ui->Zoom() / 2)) / ui->Zoom();
	y = ((ui->WindowH() / 2) - (d->H() * ui->Zoom() / 2)) / ui->Zoom();
	
	Reference middle = ui->Imageset()["dialog_c"];
	for(int xx=x; xx<x + d->W(); xx+=16) {
		for(int yy=y; yy<y + d->H(); yy+=16) {
			Draw(middle, xx, yy);
		}
	}

	Reference n = ui->Imageset()["dialog_n"],
	          s = ui->Imageset()["dialog_s"];
	for(int xx=x; xx<x + d->W(); xx+=16) {
		Draw(n, xx, y-16);
		Draw(s, xx, y+d->H());
	}

	Reference e = ui->Imageset()["dialog_e"],
	          w = ui->Imageset()["dialog_w"];
	for(int yy=y; yy<y + d->H(); yy+=16) {
		Draw(w, x-16, yy);
		Draw(e, x+d->W(), yy);
	}

	Draw(ui->Imageset()["dialog_nw"], x-16, y-16);
	Draw(ui->Imageset()["dialog_ne"], x+d->W(), y-16);
	Draw(ui->Imageset()["dialog_sw"], x-16, y+d->H());
	Draw(ui->Imageset()["dialog_se"], x+d->W(), y+d->H());
}


void
LayerDialog::DrawElement(Element const* e, int x, int y) const
{
	if(e->Type() == ELEMENT_TEXT) {
		ElementText const* et = dynamic_cast<ElementText const*>(e);
		int i = 0;
		for(char const& c: et->Text) {
			Reference rc = ui->Imageset()[string("font_").append(1, c)];
			glBindTexture(GL_TEXTURE_2D, ui->Imageset().Texture()[rc.idx]);
			float px = float(x) / 16.0f + (float(i) / (16.0f / 8.0f));
			float py = -float(y) / 16.0f - (float(et->Y) / (16.0f / 12.0f));
			glBegin(GL_QUADS);
			  glTexCoord2f(rc.x, rc.y+rc.h); 
			    glVertex3f(px, py, 0.0f);
			  glTexCoord2f(rc.x+rc.w, rc.y+rc.h); 
			    glVertex3f(px+(8.0f/16.0f), py, 0.0f);
			  glTexCoord2f(rc.x+rc.w, rc.y); 
			    glVertex3f(px+(8.0f/16.0f), py+(12.0f/16.0f), 0.0f);
			  glTexCoord2f(rc.x, rc.y); 
			    glVertex3f(px, py+(12.0f/16.0f), 0.0f);
			glEnd();
			++i;
		}
	} else {
		throw ui_error("Unsupported element type.");
	}
}
