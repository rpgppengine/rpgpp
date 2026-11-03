#include "widgets/windows/titleInputWindow.hpp"

#include "edui/confirmDialog.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"
#include "utils/editorConsts.hpp"

using namespace editor;

TitleInputWindow::TitleInputWindow() : edui::ConfirmDialog() {
	setSize({0, EDITOR_CHILD_WINDOW_WIDTH}, {0, EDITOR_CHILD_WINDOW_HEIGHT});
	centerLayout();

	auto vertLayout = std::make_shared<edui::VerticalContainer>();
	vertLayout->render->border = 0;
	vertLayout->render->as<edui::VerticalContainerRender>().space = INTERFACE_SPACING;
	vertLayout->setSize({1, 0}, {1, 0});
	vertLayout->layerId = layerId;
	this->add(vertLayout);

	auto titleLabel = std::make_shared<edui::Label>();
	titleLabel->translationId.setf("%s.Title", translationId.c_str());
	titleLabel->render->border = 0;
	titleLabel->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	titleLabel->setText("Title");
	vertLayout->add(titleLabel);

	titleBox = std::make_shared<edui::TextBox>();
	titleBox->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	titleBox->setText("");
	titleBox->onValueChanged.connect(
		[this](std::string old, std::string newText) { getOkButton().disabled = !isValid(); });
	vertLayout->add(titleBox);

	getOkButton().disabled = true;
	getOkButton().onClicked.connect([this] { onOkButton(); });
}

void TitleInputWindow::setTitleInput(const std::string &newTitle) { titleBox->setValue(newTitle); }

bool TitleInputWindow::isValid() { return !titleBox->getValue().empty(); }

void TitleInputWindow::onOkButton() {
	if (isValid()) {
		onConfirmed.invoke(titleBox->getValue());
	}
}
