#ifndef _EDUI_COLORPICKER_H
#define _EDUI_COLORPICKER_H

#include <memory>

#include "edui/colorPanel.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

namespace edui {
struct ColorPickerRender : public WidgetRender {};

struct ColorPicker : public Widget {
	SignalT2<Color, Color> onValueChanged;

	Color color = WHITE;

	std::shared_ptr<edui::ColorPanel> colorDialog = nullptr;

	void setValue(const Color &val);
	Color getValue();

	ColorPicker();

	void draw() override;
};
}  // namespace edui

#endif
