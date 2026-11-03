#include "widgets/windows/deleteConfirmWindow.hpp"

#include "edui/confirmDialog.hpp"
#include "raylib.h"
#include "utils/editorConsts.hpp"

using namespace editor;

DeleteConfirmWindow::DeleteConfirmWindow() : edui::ConfirmDialog() {
	setSize({0, EDITOR_CHILD_WINDOW_WIDTH}, {0, EDITOR_CHILD_WINDOW_HEIGHT});
	centerLayout();
	translationId = "Project.Resources.DeleteConfirm";

	this->textArea->translationId = TextFormat("%s.Text", translationId.c_str());
}
