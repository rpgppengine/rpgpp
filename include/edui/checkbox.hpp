#ifndef _EDUI_CHECKBOX_H
#define _EDUI_CHECKBOX_H

#include "edui/signal.hpp"
#include "edui/valuewidget.hpp"
#include "edui/widget.hpp"
namespace edui {
struct CheckboxRender : public WidgetRender {};

/* A widget which displays a toggleable checkbox. */
struct Checkbox : public Widget, public ValueWidgetT<bool> {
	SignalT2<bool, bool> valueChanged;

	void setValue(const bool &text) override;
	bool getValue() override;

	Checkbox();

	void draw() override;

	void leftMouseClicked() override;

private:
	bool checked = false;
};
}  // namespace edui

#endif
