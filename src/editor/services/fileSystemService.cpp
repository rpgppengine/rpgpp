#include "services/fileSystemService.hpp"

#include <array>
#include <cstdio>
#include <string>

#include "TGUI/Widgets/FileDialog.hpp"
#include "widgets/newProjectWindow.hpp"

#ifdef __linux__
#include <unistd.h>
#endif

#ifdef _WIN32
#include <winapi.hpp>
#endif

#include "editor.hpp"
#include "raylib.h"

FileSystemService::FileSystemService() {
	editorBaseDir = GetWorkingDirectory();

	/// type names
	typeNames[static_cast<int>(EngineFileType::FILE_TILESET)] = "Tilesets";
	typeNames[static_cast<int>(EngineFileType::FILE_MAP)] = "Maps";
	typeNames[static_cast<int>(EngineFileType::FILE_ACTOR)] = "Actors";
	typeNames[static_cast<int>(EngineFileType::FILE_DIALOGUE)] = "Dialogues";
	typeNames[static_cast<int>(EngineFileType::FILE_IMAGE)] = "Images";
	typeNames[static_cast<int>(EngineFileType::FILE_FONT)] = "Fonts";
	typeNames[static_cast<int>(EngineFileType::FILE_SOUND)] = "Sounds";
	typeNames[static_cast<int>(EngineFileType::FILE_MUSIC)] = "Music";
	typeNames[static_cast<int>(EngineFileType::FILE_PROP)] = "Props";
	typeNames[static_cast<int>(EngineFileType::FILE_SCRIPT)] = "Scripts";
	typeNames[static_cast<int>(EngineFileType::FILE_INTERACTABLE)] = "Interactables";
	typeNames[static_cast<int>(EngineFileType::FILE_INTERFACEVIEW)] = "Views";

	typeNames[static_cast<int>(EngineFileType::FILE_EMPTY)] = "Project Directory";

	/// type extensions
	typeExtensions[static_cast<int>(EngineFileType::FILE_TILESET)] = {".rtiles"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_MAP)] = {".rmap"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_ACTOR)] = {".ractor"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_DIALOGUE)] = {".rdiag"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_IMAGE)] = {".png", ".jpg"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_FONT)] = {".ttf"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_SOUND)] = {".wav", ".mp3"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_MUSIC)] = {".wav", ".mp3"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_PROP)] = {".rprop"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_SCRIPT)] = {".lua"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_INTERACTABLE)] = {".rinter"};
	typeExtensions[static_cast<int>(EngineFileType::FILE_INTERFACEVIEW)] = {".rui"};
}

void FileSystemService::unload() {}

void FileSystemService::promptNewProject() {
	auto newProjectDialog = NewProjectWindow::create();

	newProjectDialog->init(Editor::instance->getGui().gui.get());
	newProjectDialog->fileField->setSelectingDirectory(true);
	newProjectDialog->confirmButton->onPress([newProjectDialog] {
		std::string title = newProjectDialog->titleField->getText().toStdString();
		std::string dirPath = newProjectDialog->fileField->getChosenPath().toStdString();

		if (newProjectDialog->makeDirCheck->isChecked()) {
			std::string newDirPath = TextFormat("%s/%s", dirPath.c_str(), title.c_str());
			MakeDirectory(newDirPath.c_str());
			dirPath = newDirPath;
		}

		if (!title.empty() && !dirPath.empty()) {
			auto path = Project::create(dirPath, title);
			Project::openProject(path, true);
		}
		newProjectDialog->window->close();
	});
}

void FileSystemService::promptOpenProject() {
	auto files = tgui::FileDialog::create();
	files->setFileTypeFilters({{"RPG++ Project", {"*.rpgpp"}}});

	files->onFileSelect([](const tgui::String &filePath) { Project::openProject(filePath); });

	Editor::instance->getGui().gui->add(files);
}

std::string &FileSystemService::getTypeName(EngineFileType fileType) { return typeNames[static_cast<int>(fileType)]; }

std::array<std::string, FILETYPE_MAX> &FileSystemService::getTypeNames() { return typeNames; }

std::vector<std::string> &FileSystemService::getTypeExtensions(EngineFileType fileType) {
	return typeExtensions[static_cast<int>(fileType)];
}

std::array<std::vector<std::string>, FILETYPE_MAX> &FileSystemService::getTypeExtensions() { return typeExtensions; }

const std::string &FileSystemService::getEditorBaseDir() { return editorBaseDir; }

std::string FileSystemService::getResourcePath(const std::string &path) {
	std::filesystem::path result = editorBaseDir;
	result /= "resources";
	result /= path;

	return result.string();
}

void FileSystemService::openFileInDefaultApp(std::string &path) {
#ifdef __linux__
	auto proc = fork();
	if (proc == 0) {
		printf("%s \n", path.c_str());
		std::string command = "xdg-open";
		char *args[] = {command.data(), path.data(), nullptr};
		execvp(command.data(), args);
	}
#endif

#ifdef _WIN32
	if (!WinOpenFileAssociate("", path.c_str())) {
		printf("failure to open file...\n");
	}
#endif
}
