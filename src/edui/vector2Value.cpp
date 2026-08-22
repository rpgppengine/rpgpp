#include "edui/vector2Value.hpp"

#include <memory>

#include "edui/container.hpp"
#include "edui/intValue.hpp"
#include "raylib.h"

using namespace edui;

Vector2Value::Vector2Value() : Container() {
	render = std::make_unique<Vector2ValueRender>();
	render->border = 0;

	valX = std::make_shared<IntValue>();
	valX->setSize({0.5f, -2}, {1, 0});

	add(valX);

	valY = std::make_shared<IntValue>();
	valY->setPosition({0.5f, 2}, {0, 0});
	valY->setSize({0.5f, -2}, {1, 0});

	add(valY);

	std::weak_ptr<IntValue> weakX = valX;
	std::weak_ptr<IntValue> weakY = valY;

	valX->onValueChangedT.connect([this, weakY](int oldX, int newX) {
		if (auto sharedY = weakY.lock()) {
			int y = sharedY->getValue();

			this->onValueChangedT.invoke({static_cast<float>(oldX), static_cast<float>(y)},
										 {static_cast<float>(newX), static_cast<float>(y)});
		}
	});

	valY->onValueChangedT.connect([this, weakX](int oldY, int newY) {
		if (auto sharedX = weakX.lock()) {
			int x = sharedX->getValue();

			this->onValueChangedT.invoke({static_cast<float>(x), static_cast<float>(oldY)},
										 {static_cast<float>(x), static_cast<float>(newY)});
		}
	});
}

void Vector2Value::setValue(const Vector2 &val) {
	valX->setValue(val.x);
	valY->setValue(val.y);
}

Vector2 Vector2Value::getValue() {
	float x = valX->getValue();
	float y = valY->getValue();

	return {x, y};
}
