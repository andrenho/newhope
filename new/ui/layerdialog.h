#ifndef UI_LAYERDIALOG_H
#define UI_LAYERDIALOG_H

class LayerDialog : public Layer {
public:
	LayerDialog(Dialog** dialog) : dialog_(dialog) { }
	void Render() const;

private:
	Dialog** dialog_;
};

#endif
