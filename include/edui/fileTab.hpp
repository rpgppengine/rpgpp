#ifndef _EDUI_FILETAB_H
#define _EDUI_FILETAB_H

#include <memory>
#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/widget.hpp"
namespace edui {
struct FileTabRender : public IconTextButtonRender {};

struct FileTab : public IconTextButton {
	FileTab();

	std::shared_ptr<IconButton> closeButtonPtr = nullptr;

	void update() override;
 	void draw() override;
	void resizeToFit() override;

	void mouseEntered() override;
	void mouseLeft() override;

private:
	void processWidget(std::shared_ptr<Widget>& widget);
};
}

#endif
