#ifndef EDITOR_WIDGETS_WINDOWS_NEWRESOURCEWINDOW_H
#define EDITOR_WIDGETS_WINDOWS_NEWRESOURCEWINDOW_H

#include <memory>

#include "edui/confirmDialog.hpp"
#include "edui/signal.hpp"
#include "edui/textBox.hpp"
#include "edui/toggleButton.hpp"
#include "resource.hpp"
namespace editor {
struct NewResourceWindow : public edui::ConfirmDialog {
	edui::SignalT<std::string> onConfirmed;

	NewResourceWindow(std::unique_ptr<Resource> &ref, std::filesystem::path dirFullPath);

private:
	std::unique_ptr<Resource> &ptr;
	std::filesystem::path dirPath;

	std::shared_ptr<edui::ToggleButton> previousButton = nullptr;
	std::shared_ptr<edui::TextBox> titleBox = nullptr;
	std::string chosenType = "";
	bool hasChosenType = false;

	void initTypesList();
	void initForm();

	bool isValid();
	void onOkClicked();
};
}  // namespace editor

#endif
