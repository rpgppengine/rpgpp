#ifndef _EDUI_TABPAGE_H
#define _EDUI_TABPAGE_H

#include "container.hpp"
#include "signal.hpp"
namespace edui {
struct TabPage : public edui::Container {
	Signal onClose;

	TabPage();
};
}  // namespace edui

#endif
