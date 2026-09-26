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

static const std::string builtinTranslation = "Widgets.MessageBox";

MessageBox::MessageBox() : ChildWindow() {
	render = std::make_unique<MessageBoxRender>();
	render->padding = 4;

	textArea = std::make_shared<edui::TextArea>();
	textArea->render->border = 0;
	textArea->setSize({1, 0}, {1, 0});
	add(textArea);

	footerLayout = std::make_shared<edui::HorizontalContainer>();
	footerLayout->reverse = true;
	footerLayout->render->border = 0;
	footerLayout->render->as<edui::HorizontalContainerRender>().space = 4;
	footerLayout->gui = Gui::instance;
	footerLayout->setSize({1, 0}, {1, 0});

	auto btn = std::make_shared<edui::Button>();
	btn->setText("OK");
	btn->render->as<ButtonRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	btn->render->as<ButtonRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;

	btn->onClicked.connect([this] {
		onOkPressed.invoke();
		markDelete();
		footerLayout->markDelete();
	});

	footerLayout->add(btn);
	// btn->setWidthFit();
}

void MessageBox::translate() {
	ChildWindow::translate();

	auto &cancel = footerLayout->widgets[0];
	std::string cancelTranslationId = std::string(builtinTranslation.c_str()) + ".Ok";
	cancel->as<edui::Button>().setText(getTranslation(cancelTranslationId, "Ok"));
	cancel->as<edui::Button>().setWidthFit();
}

void MessageBox::update() {
	ChildWindow::update();

	footerLayout->update();

	auto &rend = render->as<MessageBoxRender>();

	Rectangle footerRect = {rect.x, rect.y + (rect.height - EDUI_MESSAGEBOX_FOOTER), rect.width,
							EDUI_MESSAGEBOX_FOOTER};
	footerRect.y -= rend.padding;
	footerRect.height += rend.padding;
	Rectangle paddingFooter = paddingRect(footerRect, rend.padding);
	footerLayout->calcRect(paddingFooter);

	if (footerLayout->mouseIsInRect()) {
		std::shared_ptr<Widget> widgetPtr = footerLayout;
		Gui::instance->notifyChild(&widgetPtr);
		footerLayout->notifyChildren(Gui::instance);
	} else {
		if (footerLayout->notifiedMouseEnter) {
			footerLayout->notifiedMouseEnter = false;
			footerLayout->mouseLeft();
		}
	}
}

void MessageBox::draw() {
	ChildWindow::draw();

	footerLayout->draw();
}

Rectangle MessageBox::getContentRect() {
	auto res = ChildWindow::getContentRect();
	res.height -= EDUI_MESSAGEBOX_FOOTER;
	return res;
}

void MessageBox::setText(const std::string &text) { textArea->setText(text); }
