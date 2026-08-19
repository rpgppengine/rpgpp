#ifndef _EDUI_MESSAGEBOX_H
#define _EDUI_MESSAGEBOX_H

#include <memory>

#include "edui/childWindow.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/textArea.hpp"
namespace edui {
struct MessageBoxRender : public ChildWindowRender {};

struct MessageBox : public ChildWindow {
	const float EDUI_MESSAGEBOX_FOOTER = EDUI_DEFAULT_HEIGHT + 8;

	Signal onOkPressed;

	std::shared_ptr<edui::TextArea> textArea = nullptr;
	std::shared_ptr<edui::HorizontalContainer> layout = nullptr;

	MessageBox();

	void update() override;
	void draw() override;

	Rectangle getContentRect() override;

	void setText(const std::string &text);
};
}  // namespace edui

#endif
