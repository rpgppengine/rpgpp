#include "widgets/windows/newResourceWindow.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/toggleButton.hpp"
#include "edui/verticalContainer.hpp"
#include "services/interface.hpp"
#include "utils/editorConsts.hpp"

using namespace editor;

NewResourceWindow::NewResourceWindow(std::unique_ptr<Resource> &ref, std::filesystem::path dirFullPath)
	: edui::ConfirmDialog(), ptr(ref) {
	this->dirPath = dirFullPath;

	setSize({0, EDITOR_CHILD_WINDOW_WIDTH * 2}, {0, EDITOR_CHILD_WINDOW_HEIGHT});
	centerLayout();

	translationId = "Project.Resources.NewResource";

	initTypesList();
	initForm();

	getOkButton().disabled = true;
	getOkButton().onClicked.connect([this] { onOkClicked(); });
}

void NewResourceWindow::initTypesList() {
	int offset = EDITOR_CHILD_WINDOW_WIDTH - (INTERFACE_SPACING * 2);

	auto typesLayout = std::make_shared<edui::VerticalContainer>();
	typesLayout->setSize({0, offset}, {1, 0});
	add(typesLayout);

	auto &types = Editor::getInstance().getResources().getTypes().getCategories();
	for (auto &type : types) {
		if (Editor::getInstance().getResources().getTypes().hasFactory(type.title)) {
			auto button = std::make_shared<edui::ToggleButton>();
			button->render->border = 0;
			button->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
			button->iconId = type.iconId;
			button->setText(type.title);

			std::weak_ptr<edui::ToggleButton> weakButton = button;
			button->onClicked.connect([this, weakButton, type] {
				if (auto sharedButton = weakButton.lock()) {
					if (this->previousButton != nullptr) {
						this->previousButton->setInactive();
						this->previousButton->unfocused();
					}

					this->previousButton = sharedButton;
					this->hasChosenType = true;
					this->chosenType = type.title;
					this->getOkButton().disabled = !isValid();
				}
			});

			typesLayout->add(button);
		}
	}
}

void NewResourceWindow::initForm() {
	int offset = EDITOR_CHILD_WINDOW_WIDTH - (INTERFACE_SPACING * 2);

	auto formContainer = std::make_shared<edui::VerticalContainer>();
	formContainer->render->border = 0;
	formContainer->setPosition({1, -(offset + INTERFACE_SPACING)}, {0, 0});
	formContainer->setSize({1, -(offset + (INTERFACE_SPACING))}, {1, 0});
	add(formContainer);

	auto titleLabel = std::make_shared<edui::Label>();
	titleLabel->translationId.setf("%s.Title", translationId.c_str());
	titleLabel->render->border = 0;
	titleLabel->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	titleLabel->setText("Title");
	formContainer->add(titleLabel);

	titleBox = std::make_shared<edui::TextBox>();
	titleBox->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	titleBox->setText("");
	titleBox->onValueChanged.connect(
		[this](std::string old, std::string newText) { getOkButton().disabled = !isValid(); });
	formContainer->add(titleBox);
}

bool NewResourceWindow::isValid() { return (hasChosenType && !titleBox->getValue().empty()); }

void NewResourceWindow::onOkClicked() {
	if (isValid()) {
		auto &typeManager = Editor::getInstance().getResources().getTypes();

		std::string title = titleBox->getValue();
		auto &factory = typeManager.getFactory(chosenType);
		ptr = factory.instantiate(title);

		std::filesystem::path fullPath = dirPath;
		fullPath /= std::string(title + typeManager.getCategory(chosenType).extensions[0]);
		factory.save(fullPath, ptr);

		onConfirmed.invoke(chosenType);
	}
}
