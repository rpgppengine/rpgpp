#ifndef _EDUI_CHECKBOX_H
#define _EDUI_CHECKBOX_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"
namespace edui {
struct CheckboxRender : public WidgetRender {};

/* A widget which displays a toggleable checkbox. */
struct Checkbox : public Widget {
	SignalT2<bool, bool> onValueChanged;

	void setValue(const bool &text);
	bool getValue();

	Checkbox();

	void draw() override;

	void leftMouseClicked() override;

private:
	bool checked = false;
};
}  // namespace edui

#endif
