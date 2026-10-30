#ifndef _EDITOR_WINDOWS_GAMEOUTPUT_H
#define _EDITOR_WINDOWS_GAMEOUTPUT_H

#include <memory>
#include <string>

#include "editor-new/widgets/gameOutputArea.hpp"
#include "edui/childWindow.hpp"
#include "edui/textArea.hpp"

namespace editor {
struct GameOutput : public edui::ChildWindow {
	GameOutput();

	void appendLine(const std::string &line);
	void clear();

private:
	std::shared_ptr<GameOutputArea> area = nullptr;
};
}  // namespace editor

#endif
