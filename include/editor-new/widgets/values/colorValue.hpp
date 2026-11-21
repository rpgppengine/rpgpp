#ifndef _EDITOR_WIDGETS_VALUES_COLORVALUE_H
#define _EDITOR_WIDGETS_VALUES_COLORVALUE_H

#include <memory>

#include "editor-new/widgets/values/valuewidget.hpp"
#include "edui/colorPicker.hpp"
#include "raylib.h"

namespace editor {
struct ColorValue : public ValueWidgetT<Color> {
	ColorValue();

	void setValue(const Color &value) override;
	Color getValue() override;

private:
	std::shared_ptr<edui::ColorPicker> picker;
};
}  // namespace editor

#endif
