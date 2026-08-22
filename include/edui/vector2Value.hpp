#ifndef _EDUI_VECTOR2VALUE_H
#define _EDUI_VECTOR2VALUE_H

#include <memory>

#include "edui/container.hpp"
#include "edui/intValue.hpp"
#include "edui/valuewidget.hpp"
#include "raylib.h"
namespace edui {
struct Vector2ValueRender : public ContainerRender {};

struct Vector2Value : public Container, public ValueWidgetT<Vector2> {
	std::shared_ptr<IntValue> valX;
	std::shared_ptr<IntValue> valY;

	Vector2Value();

	void setValue(const Vector2 &val) override;
	Vector2 getValue() override;
};
}  // namespace edui

#endif
