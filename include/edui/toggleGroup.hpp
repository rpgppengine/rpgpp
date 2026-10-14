#ifndef _EDUI_TOGGLEGROUP_H
#define _EDUI_TOGGLEGROUP_H

#include <memory>
#include <string>

#include "edui/horizontalContainer.hpp"
#include "edui/toggleButton.hpp"
#include "edui/widget.hpp"

namespace edui {
struct ToggleGroupRender : public HorizontalContainerRender {};

struct ToggleGroup : public HorizontalContainer {
	ToggleGroup();
	void applyTheme(const mINI::INIStructure &struc) override;

	void addItem(const std::string &text, int iconId);

	void toggleItem(int index);

private:
	std::weak_ptr<edui::Widget> activeItem;
};
}  // namespace edui

#endif
