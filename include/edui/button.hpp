#ifndef _EDUI_BUTTON_H
#define _EDUI_BUTTON_H

#include <memory>

#include "edui/label.hpp"

namespace edui {
struct ButtonRender : public LabelRender {};

/* The Button widget is a clickable button with text inside it. */
struct Button : public Label {
	typedef std::shared_ptr<Button> Ptr;

	Button();

	void draw() override;
};
}  // namespace edui

#endif
