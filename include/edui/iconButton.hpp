#ifndef _EDUI_ICONBUTTON_H
#define _EDUI_ICONBUTTON_H

#include <memory>
#include <string>

#include "edui/tooltip.hpp"
#include "edui/widget.hpp"
namespace edui {
struct IconButtonRender : public WidgetRender {};

/* A button which only displays an icon. */
struct IconButton : public Widget {
	IconButton();
	void translate() override;

	int iconId = 0;

	void draw() override;
	void mouseEntered() override;
	void mouseLeft() override;

	void setTooltip();
	void setTooltip(const std::string &text);

private:
	bool hasTooltip = false;
	std::string tooltipText = "";
	std::shared_ptr<Tooltip> tooltip = nullptr;
};
}  // namespace edui

#endif
