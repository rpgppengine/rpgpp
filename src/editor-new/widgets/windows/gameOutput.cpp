#include "widgets/windows/gameOutput.hpp"

#include <memory>

#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "widgets/gameOutputArea.hpp"

using namespace editor;

GameOutput::GameOutput() {
	setSize({0, 600}, {0, 450});

	auto textArea = std::make_shared<GameOutputArea>();
	textArea->setSize({1, 0}, {1, 0});

	this->area = textArea;
	add(textArea);
}

void GameOutput::update() {
	edui::ChildWindow::update();

	auto &process = Editor::getInstance().getResources().getProject().getProcess();
	if (process.isRunning() && process.hasData()) {
		auto read = process.readOut();
		appendLine(read);
	}
}

void GameOutput::appendLine(const std::string &line) {
	if (area != nullptr) {
		area->appendLine(line);
		area->scrollToBottom();
	}
}

void GameOutput::clear() {
	if (area != nullptr) {
		area->clear();
	}
}
