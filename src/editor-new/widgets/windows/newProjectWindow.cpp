#include "widgets/windows/newProjectWindow.hpp"

#include <memory>
#include <string>

#include "Str.h"
#include "editor.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/fileValue.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/label.hpp"
#include "edui/textArea.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "raylib.h"
#include "services/interface.hpp"

using namespace editor;

NewProjectWindow::NewProjectWindow() : edui::ConfirmDialog() {
	setSize({0, 300}, {0, 250});
	centerLayout();
	translationId = "MainMenu.NewProject";

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

	auto dirLabel = std::make_shared<edui::Label>();
	dirLabel->translationId.setf("%s.Folder", translationId.c_str());
	dirLabel->render->border = 0;
	dirLabel->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	dirLabel->setText("Folder");
	vertLayout->add(dirLabel);

	dirValue = std::make_shared<edui::FileValue>();
	dirValue->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	dirValue->setDirSelection();
	vertLayout->add(dirValue);

	auto dirPathLabel = std::make_shared<edui::TextArea>();
	dirPathLabel->render->border = 0;
	dirPathLabel->setSize({1, 0}, {0, 2 * edui::EDUI_SECONDARY_HEIGHT});
	dirPathLabel->setText("");

	std::weak_ptr<edui::TextArea> weakArea = dirPathLabel;
	dirValue->onValueChanged.connect([this, weakArea](edui::FileOption old, edui::FileOption newFileValue) {
		if (auto sharedArea = weakArea.lock()) {
			std::string key = TextFormat("%s.ProjectAt", translationId.c_str());
			std::string newText = getTranslation(key, "Project at") + " " + newFileValue.fullPath;
			sharedArea->setText(newText);
		}
		getOkButton().disabled = !isValid();
	});
	dirValue->fileValueEmptied.connect([this, weakArea] {
		if (auto sharedArea = weakArea.lock()) {
			sharedArea->setText("");
		}
		getOkButton().disabled = !isValid();
	});

	vertLayout->add(dirPathLabel);

	getOkButton().disabled = true;

	getOkButton().onClicked.connect([this] { this->onOkButton(); });
}

bool NewProjectWindow::isValid() {
	std::string titleText = titleBox->getValue();
	edui::FileOption dir = dirValue->getValue();

	return !titleText.empty() && dir;
}

void NewProjectWindow::onOkButton() {
	std::string titleText = titleBox->getValue();
	edui::FileOption dir = dirValue->getValue();

	if (isValid()) {
		Editor::getInstance().getResources().getProject().create(titleText, dir.fullPath);
	}
}
