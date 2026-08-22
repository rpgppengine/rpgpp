#include "edui/confirmDialog.hpp"

#include <memory>

#include "edui/button.hpp"
#include "edui/messageBox.hpp"

using namespace edui;

ConfirmDialog::ConfirmDialog() : MessageBox() {
	render = std::make_unique<ConfirmDialogRender>();
	render->padding = 4;

	auto cancel = std::make_shared<edui::Button>();
	cancel->setSize({0, 70}, {1, 0});
	cancel->setText("Cancel");
	cancel->render->as<ButtonRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	cancel->render->as<ButtonRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;

	cancel->onClicked.connect([this] {
		onCancelPressed.invoke();
		markDelete();
		layout->markDelete();
	});

	layout->add(cancel);

	layout->widgets[0].swap(layout->widgets[1]);
}
