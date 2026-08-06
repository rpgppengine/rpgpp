#ifndef _EDUI_CHECKBOX_H
#define _EDUI_CHECKBOX_H

#include "edui/signal.hpp"
#include "edui/valuewidget.hpp"
#include "edui/widget.hpp"
namespace edui {
struct CheckboxRender : public WidgetRender {};

struct Checkbox : public Widget, public ValueWidgetT<bool> {
	SignalT2<bool, bool> valueChanged;

	bool checked = false;

	void setValue(const bool &text) override;
	bool getValue() override;

	Checkbox();

	void draw() override;

	void leftMouseClicked() override;
};
}  // namespace edui

#endif
