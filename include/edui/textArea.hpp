#ifndef _EDUI_TEXTAREA_H
#define _EDUI_TEXTAREA_H

#include "edui/label.hpp"
#include "edui/signal.hpp"
#include "edui/widget.hpp"
namespace edui {
struct TextAreaRender : public LabelRender {};

struct TextArea : public Widget {
	TextArea();

	std::string text = "Hello!";

	void draw() override;

	void setText(const std::string &text);
};
}  // namespace edui

#endif
