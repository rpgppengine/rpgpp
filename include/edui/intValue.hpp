#ifndef _EDUI_INTVALUE_H
#define _EDUI_INTVALUE_H

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/signal.hpp"
namespace edui {
struct IntValueRender : public ContainerRender {
	Color btnColor;
};

struct IntValue : public Container {
	IntValueTextBox::Ptr intval;
	Button::Ptr incButton;
	Button::Ptr decButton;

	SignalT2<int, int> valueChanged;

	IntValue();
};
}  // namespace edui

#endif
