#include "childWindows/elementRenameWindow.hpp"
#include "actions/action.hpp"
#include "bindTranslation.hpp"
#include "screens/projectScreen.hpp"

void ElementRenameWindow::onRenameElement(ElementIndex index, const std::string& newName) {
	if (view == nullptr) return;
	if (tree == nullptr) return;

	std::string previousName = view->getEntityName(index);

	auto action = std::make_unique<Action>();
	action->onAction = [this, previousName, newName] {
		view->renameElement(previousName, newName);
		tree->changeItem({previousName}, newName);
	};
	action->onUndo = [this, previousName, newName] {
		view->renameElement(newName, previousName);
		tree->changeItem({newName}, previousName);
	};

	auto screen = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
	screen->getCurrentFile().getView().pushAction(std::move(action));
}

ElementRenameWindow::ElementRenameWindow() : PopupWindow("Rename Element..") {
	bindTranslation(this->currentWindow, "dialog.rename_element.title", &tgui::ChildWindow::setTitle);

	view = nullptr;
	currentWindow->setSize(280, 280);

	input = tgui::EditBox::create();
	bindTranslation(input, "dialog.init_element.elm_name", &tgui::EditBox::setDefaultText);
	input->setSize({"100%", 32});

	auto submit = tgui::Button::create();
	bindTranslation<tgui::Button>(submit, "dialog.rename_element.rename", &tgui::Button::setText);
	submit->setPosition({0, 36});
	submit->setSize("100%", 32);

	std::weak_ptr<tgui::EditBox> weakInput = input;
	submit->onPress([this, weakInput] {
		std::string newName = input->getText().toStdString();

		if (newName.empty()) return;
		if (elementIndex == MAX_ELEMENTS) return;

		onRenameElement(elementIndex, newName);

		close();
	});

	currentWindow->add(input);
	currentWindow->add(submit);
}

void ElementRenameWindow::init() {
	input->setText("");
}
