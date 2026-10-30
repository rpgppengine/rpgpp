#include "utils/project.hpp"

#include <cstdio>
#include <filesystem>
#include <memory>

#include "editor.hpp"
#include "edui/gui.hpp"
#include "ini.h"
#include "raylib.h"
#include "widgets/windows/gameOutput.hpp"

using namespace editor;

Project::Project() {}

void Project::update() { process.update(); }

void Project::openFromPath(const std::string &path) {
	if (!FileExists(path.c_str())) {
		Editor::getInstance().getInterface().showErrorMessage("File does not exist.");
		return;
	}

	mINI::INIFile file(path);
	mINI::INIStructure struc;

	bool readResult = file.read(struc);
	if (!readResult) {
		Editor::getInstance().getInterface().showErrorMessage("Cannot read this file");
		return;
	}

	this->title = struc.get("Project").get("Title");

	basePath = GetDirectoryPath(path.c_str());
	auto recent = Editor::getInstance().getResources().getRecentProjects();
	recent.enqueue(path);

	ChangeDirectory(basePath.c_str());

	projectOpened.invoke();
}

void Project::create(const std::string &title, const std::string &dirPath) {
	std::filesystem::path stdPath = dirPath;
	stdPath /= title;

	if (DirectoryExists(stdPath.c_str())) {
		Editor::getInstance().getInterface().showErrorMessage("Directory already exists.");
		return;
	}

	MakeDirectory(stdPath.c_str());
	basePath = stdPath;

	std::filesystem::path iniPath = stdPath / "proj.rpgpp";

	mINI::INIFile file(iniPath);
	mINI::INIStructure struc;

	auto &section = struc["Project"];
	section["Title"] = title;

	file.write(struc);

	openFromPath(iniPath);
}

void Project::spawnGame() {
	if (process.isRunning()) return;

	std::filesystem::path stdPath = basePath;
	stdPath /= title;

	const char *line[] = {stdPath.c_str(), NULL};
	int res = process.spawn(line);

	if (res != 0) return;

	if (!outputOpen) {
		outputWindow = std::make_shared<GameOutput>();
		outputOpen = true;

		process.out.disconnectAll();

		std::weak_ptr<GameOutput> weakWindow = outputWindow;
		process.out.connect([weakWindow](std::string line) {
			if (auto sharedWindow = weakWindow.lock()) {
				sharedWindow->appendLine(line);
			}
		});
	}
	outputWindow->clear();

	outputWindow->centerLayout();
	outputWindow->onDeleted.connect([this] { outputOpen = false; });
	Editor::getInstance().getInterface().getEdui().add(outputWindow, edui::EDUI_MAX_LAYERS - 1);
}

std::string Project::getTitle() { return title; }

void Project::setTitle(const std::string &title) { this->title = title; }
