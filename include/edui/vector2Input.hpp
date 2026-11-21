#ifndef _EDUI_VECTOR2INPUT_H
#define _EDUI_VECTOR2INPUT_H

#include <memory>

#include "edui/container.hpp"
#include "edui/numberInput.hpp"
#include "raylib.h"
namespace edui {
struct Vector2InputRender : public ContainerRender {};

struct Vector2Input : public Container {
	SignalT2<Vector2, Vector2> onValueChanged;

	std::shared_ptr<NumberInput> valX;
	std::shared_ptr<NumberInput> valY;

	Vector2Input();

	void setValue(const Vector2 &val);
	Vector2 getValue();
};
}  // namespace edui

#endif
