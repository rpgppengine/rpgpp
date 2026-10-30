#include "widgets/gameOutputArea.hpp"

#include "edui/textEdit.hpp"

using namespace editor;

GameOutputArea::GameOutputArea() : edui::TextEdit() { setEditable(false); }

void GameOutputArea::appendLine(const std::string &line) {
	text = this->text.append(line);
	reloadLines();
}

void GameOutputArea::clear() {
	text = "Game Started..";
	reloadLines();
}
