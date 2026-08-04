#ifndef _EDUI_BUTTON_H
#define _EDUI_BUTTON_H

#include <memory>
#include <string>

#include "edui/label.hpp"
#include "edui/signal.hpp"
#include "edui/widget.hpp"

namespace edui {
struct ButtonRender : public LabelRender {};

struct Button : public Label {
	typedef std::shared_ptr<Button> Ptr;

	Button();
};
}  // namespace edui

#endif
