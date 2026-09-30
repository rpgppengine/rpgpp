#ifndef _EDUI_HORIZONTALSCROLLBAR_H
#define _EDUI_HORIZONTALSCROLLBAR_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"
namespace edui {
struct HorizontalScrollbarRender : public WidgetRender {};

struct HorizontalScrollbar : public Widget {
	SignalT<int> onValueChanged;

	int maxScroll = 100;

	HorizontalScrollbar();
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
