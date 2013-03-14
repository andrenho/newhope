#include "defines.h"

void
LayerDialog::Render() const
{
	if(!*dialog_) {
		return;
	}

	ui->Scene().setProportion(PROP_WINDOW);

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
	x = ((ui->WindowZoomW()/2) - (d->W() / 2));
	y = ((ui->WindowZoomH()/2) - (d->H() / 2));

	for(int xx=x; xx<x + d->W(); xx+=16) {
		for(int yy=y; yy<y + d->H(); yy+=16) {
			ui->Scene().DrawImage("dialog_c", xx, yy);
		}
	}

	for(int xx=x; xx<x + d->W(); xx+=16) {
		ui->Scene().DrawImage("dialog_n", xx, y-16);
		ui->Scene().DrawImage("dialog_s", xx, y+d->H());
	}

	for(int yy=y; yy<y + d->H(); yy+=16) {
		ui->Scene().DrawImage("dialog_w", x-16, yy);
		ui->Scene().DrawImage("dialog_e", x+d->W(), yy);
	}

	ui->Scene().DrawImage("dialog_nw", x-16, y-16);
	ui->Scene().DrawImage("dialog_ne", x+d->W(), y-16);
	ui->Scene().DrawImage("dialog_sw", x-16, y+d->H());
	ui->Scene().DrawImage("dialog_se", x+d->W(), y+d->H());
}


void
LayerDialog::DrawElement(Element const* e, int x, int y) const
{
	if(e->Type() == ELEMENT_TEXT) {
		ElementText const* et = dynamic_cast<ElementText const*>(e);
		int i = 0;
		for(char const& c: et->Text) {
			ui->Scene().DrawImage(string("font_").append(1, c),
					x + (i * 8), y + (et->Y * 12));
			++i;
		}
	} else {
		throw ui_error("Unsupported element type.");
	}
}
