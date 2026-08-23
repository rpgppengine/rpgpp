#ifndef _EDUI_COLORWHEEL_H
#define _EDUI_COLORWHEEL_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
namespace edui {
struct ColorWheelRender : public WidgetRender {};

struct ColorWheel : public Widget {
	SignalT<Color> onColorChanged;

	Vector2 circlePosition = {0, 0};
	Vector2 posScale = {0.5f, 0.5f};

	bool holdingMouse = false;

	ColorWheel();

	void update() override;
	void draw() override;

	void leftMouseClicked() override;
	void leftMouseReleased() override;

	void setCirclePosToCenter();

	void setColor(Color c);
	void setValue(float newValue);
	float getValue();

private:
	// Slider value, literally maps to value in HSV
	float colorValue = 1.0f;
};
}  // namespace edui

#endif
