#ifndef _EDUI_INTVALUE_H
#define _EDUI_INTVALUE_H

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/signal.hpp"
#include "edui/valuewidget.hpp"

namespace edui {
struct IntValueRender : public ContainerRender {
	Color btnColor;
};

struct IntValue : public Container, public ValueWidgetT<int> {
	IntValueTextBox::Ptr intval;
	Button::Ptr incButton;
	Button::Ptr decButton;

	void setValue(const int &val) override;
	int getValue() override;

	IntValue();
};
}  // namespace edui

#endif
