#ifndef _EDUI_CHECKBOX_H
#define _EDUI_CHECKBOX_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"
namespace edui {
struct CheckboxRender : public WidgetRender {};

struct Checkbox : public Widget {
	SignalT2<bool, bool> valueChanged;

	bool checked = false;

	Checkbox();

	void draw() override;

	void leftMouseClicked() override;
};
}  // namespace edui

#endif
