#include "widgets/filePanels/filePanel.hpp"

#include <filesystem>
#include <fstream>
#include <utility>

#include "cereal/archives/json.hpp"
#include "editor.hpp"
#include "edui/container.hpp"
#include "raylib.h"
#include "utils/commandHistory.hpp"

using namespace editor;

FilePanel::FilePanel() : edui::Container() { render->border = 0; }

void FilePanel::setResource(std::unique_ptr<Resource> ptr) { resource.swap(ptr); }

void FilePanel::setSourcePath(const std::filesystem::path &path) { this->sourcePath = path; }

std::filesystem::path FilePanel::getSourcePath() { return sourcePath; }

CommandHistory &FilePanel::getHistory() { return history; }

void FilePanel::undo() {
	history.undo();
	dirty.invoke(true);
	refresh();
}

void FilePanel::redo() {
	history.redo();
	dirty.invoke(true);
	refresh();
}

void FilePanel::saveFile() {
	auto &types = Editor::getInstance().getResources().getTypes();
	std::string title;
	bool result = types.getTypeTitleByExtension(GetFileExtension(sourcePath.c_str()), &title);
	if (result) {
		auto &factory = types.getFactory(title);
		factory.save(sourcePath, resource);
		dirty.invoke(false);
	}
}

void FilePanel::exec(std::unique_ptr<Command> cmd) {
	history.exec(std::move(cmd));
	dirty.invoke(true);
	//	resource->loadFile();
	refresh();
}
