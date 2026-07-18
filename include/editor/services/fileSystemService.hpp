#ifndef _RPGPP_FILESYSTEMSERVICE_H
#define _RPGPP_FILESYSTEMSERVICE_H

#include <array>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>

#include "TGUI/String.hpp"
#include "variant.hpp"

#define FILETYPE_MAX 13

enum class EngineFileType {
	FILE_TILESET,
	FILE_MAP,
	FILE_SCRIPT,
	FILE_ACTOR,
	FILE_DIALOGUE,
	FILE_IMAGE,
	FILE_FONT,
	FILE_SOUND,
	FILE_MUSIC,
	FILE_PROP,
	FILE_INTERACTABLE,
	FILE_INTERFACEVIEW,
	FILE_EMPTY,
};

class FileSystemService {
private:
	std::array<std::string, FILETYPE_MAX> typeNames;
	std::array<std::vector<std::string>, FILETYPE_MAX> typeExtensions;
	std::string editorBaseDir;

public:
	FileSystemService();
	void unload();
	void promptNewProject();
	void promptOpenProject();
	std::string &getTypeName(EngineFileType fileType);
	std::array<std::string, FILETYPE_MAX> &getTypeNames();
	std::vector<std::string> &getTypeExtensions(EngineFileType type);
	std::array<std::vector<std::string>, FILETYPE_MAX> &getTypeExtensions();
	const std::string &getEditorBaseDir();
	std::string getResourcePath(const std::string &path);
	void openFileInDefaultApp(std::string &path);
};

#endif
