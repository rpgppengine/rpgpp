#ifndef _EDUI_COLORVALUE_H
#define _EDUI_COLORVALUE_H

#include <memory>

#include "edui/colorPanel.hpp"
#include "edui/valuewidget.hpp"
#include "edui/widget.hpp"

namespace edui {
struct ColorValueRender : public WidgetRender {};

struct ColorValue : public Widget, public ValueWidgetT<Color> {
	Color color = WHITE;

	std::shared_ptr<edui::ColorPanel> colorDialog = nullptr;

	void setValue(const Color &val) override;
	Color getValue() override;

	ColorValue();

	void draw() override;
};
}  // namespace edui

#endif
