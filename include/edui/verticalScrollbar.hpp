#ifndef _EDUI_VERTICALSCROLLBAR_H
#define _EDUI_VERTICALSCROLLBAR_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"

namespace edui {
struct VerticalScrollbarRender : public WidgetRender {};

struct VerticalScrollbar : public Widget {
	SignalT<int> onValueChanged;

	int maxScroll = 100;

	VerticalScrollbar();

	void update() override;
	void draw() override;
	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void setMaxScroll(int newValue);
	void setValue(int value);

private:
	int currentValue = 0;
	bool scrolling = false;
	int mouseOffset = 0;
	Rectangle getBarRect();
	void drawBar();
	void setValueFromMouse();
};
}  // namespace edui

#endif
