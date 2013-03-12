#ifndef UI_LAYERDIALOG_H
#define UI_LAYERDIALOG_H

class LayerDialog : public Layer {
public:
	LayerDialog(Dialog** dialog) : dialog_(dialog) { }
	void Render() const;

private:
	void DrawBackground(int& pos_x, int& pos_y) const;
	void DrawElement(Element const* e, int x, int y) const;

	Dialog** dialog_;
};

#endif
