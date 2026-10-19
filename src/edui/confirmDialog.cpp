#include "edui/confirmDialog.hpp"

#include <memory>
#include <string>

#include "edui/button.hpp"
#include "edui/helper.hpp"
#include "edui/messageBox.hpp"

using namespace edui;

static const std::string builtinTranslation = "Widgets.ConfirmDialog";

ConfirmDialog::ConfirmDialog() : MessageBox() {
	render = std::make_unique<ConfirmDialogRender>();
	render->padding = 4;

	auto cancel = std::make_shared<edui::Button>();
	cancel->referId = CANCEL_BUTTON_ID;
	cancel->setSize({0, 70}, {1, 0});
	cancel->setText("Cancel");
	cancel->render->padding = 8;
	cancel->render->as<ButtonRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	cancel->render->as<ButtonRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;

	cancel->onClicked.connect([this] {
		onCancelPressed.invoke();
		markDelete();
		footerLayout->markDelete();
	});

	footerLayout->add(cancel);

	footerLayout->widgets[0].swap(footerLayout->widgets[1]);
}

void ConfirmDialog::translate() {
	MessageBox::translate();

	// ok button
	auto &ok = getOkButton();
	std::string okTranslationId = std::string(builtinTranslation.c_str()) + ".Ok";
	ok.setText(getTranslation(okTranslationId, "OK"));
	ok.resizeToFit();

	// cancel button
	auto &cancel = getCancelButton();
	std::string cancelTranslationId = std::string(builtinTranslation.c_str()) + ".Cancel";
	cancel.setText(getTranslation(cancelTranslationId, "Cancel"));
	cancel.resizeToFit();

	footerLayout->updateWidgets();
}

edui::Button &ConfirmDialog::getCancelButton() {
	edui::Button &btn = footerLayout->getWidgetById(CANCEL_BUTTON_ID)->as<Button>();
	return btn;
}
