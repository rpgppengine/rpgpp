#ifndef _EDITOR_WIDGETS_VALUES_RECTANGLEVALUE_H
#define _EDITOR_WIDGETS_VALUES_RECTANGLEVALUE_H

#include <memory>

#include "edui/rectangleInput.hpp"
#include "raylib.h"
#include "widgets/values/valuewidget.hpp"
namespace editor {
struct RectangleValue : ValueWidgetT<Rectangle> {
	RectangleValue();

	Rectangle getValue() override;
	void setValue(const Rectangle &value) override;

private:
	std::shared_ptr<edui::RectangleInput> input;
};
}  // namespace editor

#endif
