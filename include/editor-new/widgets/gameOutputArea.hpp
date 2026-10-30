#ifndef _EDUI_WIDGETS_GAMEOUTPUTAREA_H
#define _EDUI_WIDGETS_GAMEOUTPUTAREA_H

#include <string>

#include "edui/textArea.hpp"
#include "edui/textEdit.hpp"
namespace editor {
struct GameOutputArea : public edui::TextEdit {
	GameOutputArea();

	void appendLine(const std::string &line);
	void clear();
};
}  // namespace editor

#endif
