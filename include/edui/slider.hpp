#ifndef _EDUI_SLIDER_H
#define _EDUI_SLIDER_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"
namespace edui {
struct SliderRender : public WidgetRender {};

struct Slider : public Widget {
	const float EDUI_SLIDER_WIDTH = 16.0f;
	const float EDUI_SLIDER_BAR_OFFSET = 2.0f;

	SignalT<int> onValueChanged;

	int value = 50;

	bool holdingMouse = false;
	Vector2 barOffset = {-1, 0};

	Slider();

	void setMinValue(int newMinValue);
	void setMaxValue(int newMaxValue);
	void setValue(int newValue);

	void draw() override;
	void update() override;

	void leftMouseClicked() override;
	void leftMouseReleased() override;

private:
	int minValue = 0;
	int maxValue = 100;

	Rectangle getSliderRect();
	void setValueByMouse();
};
}  // namespace edui

#endif
