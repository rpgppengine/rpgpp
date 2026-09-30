#ifndef _EDUI_FILETAB_H
#define _EDUI_FILETAB_H

#include <memory>

#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/tab.hpp"
#include "edui/widget.hpp"

namespace edui {
struct FileTabRender : public TabRender {};

struct FileTab : public Tab {
	std::shared_ptr<IconButton> closeButtonPtr = nullptr;
	bool active = false;
	bool closeEnabled = true;

	FileTab();
	void applyTheme(const mINI::INIStructure &struc) override;

	void update() override;
	void draw() override;
	void resizeToFit() override;

	void mouseEntered() override;
	void mouseLeft() override;

	void setActive() override;
	void setInactive() override;

private:
	void processWidget(std::shared_ptr<Widget> &widget);
};
}  // namespace edui

#endif
