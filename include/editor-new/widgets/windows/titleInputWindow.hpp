#ifndef _EDITOR_WIDGETS_WINDOWS_TITLEINPUTWINDOW_H
#define _EDITOR_WIDGETS_WINDOWS_TITLEINPUTWINDOW_H

#include "edui/confirmDialog.hpp"
#include "edui/signal.hpp"
#include "edui/textBox.hpp"

namespace editor {
struct TitleInputWindow : public edui::ConfirmDialog {
	edui::SignalT<std::string> onConfirmed;

	TitleInputWindow();
	void setTitleInput(const std::string &newTitle);

private:
	std::shared_ptr<edui::TextBox> titleBox = nullptr;
	bool isValid();
	void onOkButton();
};
}  // namespace editor

#endif
