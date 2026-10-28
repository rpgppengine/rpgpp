#ifndef _EDITOR_WIDGETS_WINDOWS_NEWPROJECTWINDOW_H
#define _EDITOR_WIDGETS_WINDOWS_NEWPROJECTWINDOW_H

#include <memory>

#include "edui/confirmDialog.hpp"
#include "edui/fileValue.hpp"
#include "edui/textBox.hpp"

namespace editor {
struct NewProjectWindow : public edui::ConfirmDialog {
	NewProjectWindow();

private:
	std::shared_ptr<edui::TextBox> titleBox = nullptr;
	std::shared_ptr<edui::FileValue> dirValue = nullptr;
	bool isValid();
	void onOkButton();
};
}  // namespace editor

#endif
