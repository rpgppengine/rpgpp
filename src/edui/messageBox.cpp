#include "edui/messageBox.hpp"

#include <memory>

#include "edui/button.hpp"
#include "edui/childWindow.hpp"
#include "edui/helper.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/label.hpp"
#include "edui/textArea.hpp"
#include "edui/verticalContainer.hpp"

using namespace edui;

MessageBox::MessageBox() : ChildWindow() {
	render = std::make_unique<MessageBoxRender>();
	render->padding = 4;

	textArea = std::make_shared<edui::TextArea>();
	textArea->render->border = 0;
	textArea->setSize({1, 0}, {1, 0});
	add(textArea);

	layout = std::make_shared<edui::HorizontalContainer>();
	layout->reverse = true;
	layout->render->border = 0;
	layout->render->as<edui::HorizontalContainerRender>().space = 4;
	layout->gui = Gui::instance;
	layout->setSize({1, 0}, {1, 0});

	auto btn = std::make_shared<edui::Button>();
	btn->setText("OK");
	btn->render->as<ButtonRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	btn->render->as<ButtonRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;

	btn->clicked = [this] {
		onOkPressed.invoke();
		markDelete();
		layout->markDelete();
	};

	layout->add(btn);
}

void MessageBox::update() {
	ChildWindow::update();

	layout->update();

	auto &rend = render->as<MessageBoxRender>();

	Rectangle footerRect = {rect.x, rect.y + (rect.height - EDUI_MESSAGEBOX_FOOTER), rect.width,
							EDUI_MESSAGEBOX_FOOTER};
	footerRect.y -= rend.padding;
	footerRect.height += rend.padding;
	Rectangle paddingFooter = paddingRect(footerRect, rend.padding);
	layout->calcRect(paddingFooter);

	if (layout->mouseIsInRect()) {
		std::shared_ptr<Widget> widgetPtr = layout;
		Gui::instance->notifyChild(&widgetPtr);
		layout->notifyChildren(Gui::instance);
	} else {
		if (layout->notifiedMouseEnter) {
			layout->notifiedMouseEnter = false;
			layout->mouseLeft();
		}
	}
}

void MessageBox::draw() {
	ChildWindow::draw();

	layout->draw();
}

Rectangle MessageBox::getContentRect() {
	auto res = ChildWindow::getContentRect();
	res.height -= EDUI_MESSAGEBOX_FOOTER;
	return res;
}

void MessageBox::setText(const std::string &text) { textArea->setText(text); }
