#ifndef _EDUI_NUMBERINPUT_H
#define _EDUI_NUMBERINPUT_H

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/intValueTextBox.hpp"

namespace edui {
struct IntValueRender : public ContainerRender {
	Color btnColor;
};

struct NumberInput : public Container {
	SignalT2<int, int> onValueChanged;

	IntValueTextBox::Ptr intval;
	Button::Ptr incButton;
	Button::Ptr decButton;

	void setValue(const int &val);
	int getValue();

	NumberInput();

	void draw() override;
};
}  // namespace edui

#endif
