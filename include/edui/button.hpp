#ifndef _EDUI_BUTTON_H
#define _EDUI_BUTTON_H

#include <string>

#include "edui/signal.hpp"
#include "edui/widget.hpp"
#include "edui/label.hpp"

namespace edui {
struct ButtonRender : public LabelRender {};

struct Button : public Label {
	Button();
};
}

#endif
