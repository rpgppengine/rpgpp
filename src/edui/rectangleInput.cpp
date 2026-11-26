#include "edui/rectangleInput.hpp"

#include <memory>

#include "numberInput.hpp"

using namespace edui;

RectangleInput::RectangleInput() {
	render = std::make_unique<RectangleInputRender>();
	render->border = 0;

	valX = std::make_shared<NumberInput>();
	valX->setSize({0.5, 0}, {0.5, 0});
	valX->onValueChanged.connect([this](int a, int b) {
		auto oldRect = rect;
		this->rect.x = b;
		onValueChanged.invoke(oldRect, rect);
	});

	valY = std::make_shared<NumberInput>();
	valY->setPosition({0.5, 0}, {0, 0});
	valY->setSize({0.5, 0}, {0.5, 0});
	valY->onValueChanged.connect([this](int a, int b) {
		auto oldRect = rect;
		this->rect.y = b;
		onValueChanged.invoke(oldRect, rect);
	});

	valWidth = std::make_shared<NumberInput>();
	valWidth->setPosition({0, 0}, {0.5, 0});
	valWidth->setSize({0.5, 0}, {0.5, 0});
	valWidth->onValueChanged.connect([this](int a, int b) {
		auto oldRect = rect;
		this->rect.width = b;
		onValueChanged.invoke(oldRect, rect);
	});

	valHeight = std::make_shared<NumberInput>();
	valHeight->setPosition({0.5, 0}, {0.5, 0});
	valHeight->setSize({0.5, 0}, {0.5, 0});
	valHeight->onValueChanged.connect([this](int a, int b) {
		auto oldRect = rect;
		this->rect.height = b;
		onValueChanged.invoke(oldRect, rect);
	});

	add(valX);
	add(valY);
	add(valWidth);
	add(valHeight);
}

Rectangle RectangleInput::getValue() { return rect; }

void RectangleInput::setValue(const Rectangle &value) {
	this->rect = value;

	valX->setValue(rect.x);
	valY->setValue(rect.y);
	valWidth->setValue(rect.width);
	valHeight->setValue(rect.height);
}
