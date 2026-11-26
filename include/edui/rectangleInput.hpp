#ifndef _EDUI_RECTANGLEINPUT_H
#define _EDUI_RECTANGLEINPUT_H

#include <memory>

#include "container.hpp"
#include "numberInput.hpp"
#include "raylib.h"
#include "signal.hpp"
namespace edui {
struct RectangleInputRender : public ContainerRender {};

struct RectangleInput : public Container {
	SignalT2<Rectangle, Rectangle> onValueChanged;

	std::shared_ptr<NumberInput> valX;
	std::shared_ptr<NumberInput> valY;
	std::shared_ptr<NumberInput> valWidth;
	std::shared_ptr<NumberInput> valHeight;

	RectangleInput();

	void setValue(const Rectangle &value);
	Rectangle getValue();

private:
	Rectangle rect = {0, 0, 0, 0};
};
}  // namespace edui

#endif
