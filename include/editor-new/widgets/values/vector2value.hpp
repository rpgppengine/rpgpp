#ifndef _EDITOR_WIDGETS_VALUES_VECTOR2VALUE_H
#define _EDITOR_WIDGETS_VALUES_VECTOR2VALUE_H

#include <memory>

#include "editor-new/widgets/values/valuewidget.hpp"
#include "edui/vector2Input.hpp"
#include "raylib.h"

namespace editor {
struct Vector2Value : public ValueWidgetT<Vector2> {
	Vector2Value();

	void setValue(const Vector2 &value) override;
	Vector2 getValue() override;

private:
	std::shared_ptr<edui::Vector2Input> input;
};
}  // namespace editor

#endif
