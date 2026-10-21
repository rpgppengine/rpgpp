#ifndef _EDUI_MESSAGEBOX_H
#define _EDUI_MESSAGEBOX_H

#include <memory>

#include "edui/button.hpp"
#include "edui/childWindow.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/textArea.hpp"
namespace edui {
struct MessageBoxRender : public ChildWindowRender {};

/* A ChildWindow which has an 'OK' button. */
struct MessageBox : public ChildWindow {
	const float EDUI_MESSAGEBOX_FOOTER = EDUI_SECONDARY_HEIGHT;
	const int OK_BUTTON_ID = 1;

	Signal onOkPressed;

	std::shared_ptr<edui::TextArea> textArea = nullptr;
	std::shared_ptr<edui::HorizontalContainer> footerLayout = nullptr;

	MessageBox();
	void applyTheme(const mINI::INIStructure &struc) override;
	void translate() override;

	void update() override;
	void draw() override;

	Rectangle getContentRect() override;

	/* Set the text of the message. */
	void setText(const std::string &text);

	/* Get a reference to the 'OK' button. */
	edui::Button &getOkButton();

protected:
	Rectangle footerRect = {0, 0, 0, 0};
};
}  // namespace edui

#endif
