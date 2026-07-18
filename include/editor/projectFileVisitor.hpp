#ifndef _RPGPP_PROJECTFILEVISITOR_H
#define _RPGPP_PROJECTFILEVISITOR_H

#include <array>
#include <functional>
#include <memory>
#include <string>

#include "projectFile.hpp"
#include "services/fileSystemService.hpp"

class ProjectFileVisitor {
private:
	std::array<std::function<std::unique_ptr<ProjectFile>(const std::string &)>, FILETYPE_MAX> funcs;

public:
	ProjectFileVisitor();
	std::unique_ptr<ProjectFile> visit(EngineFileType fileType, const std::string &path);
	static std::unique_ptr<ProjectFile> emptyView(const std::string &path);
	static std::unique_ptr<ProjectFile> tilesetView(const std::string &path);
	static std::unique_ptr<ProjectFile> roomView(const std::string &path);
	static std::unique_ptr<ProjectFile> codeView(const std::string &path);
	static std::unique_ptr<ProjectFile> dialogueView(const std::string &path);
	static std::unique_ptr<ProjectFile> propView(const std::string &path);
	static std::unique_ptr<ProjectFile> actorView(const std::string &path);
	static std::unique_ptr<ProjectFile> imageView(const std::string &path);
	static std::unique_ptr<ProjectFile> fontView(const std::string &path);
	static std::unique_ptr<ProjectFile> soundView(const std::string &path);
	static std::unique_ptr<ProjectFile> musicView(const std::string &path);
	static std::unique_ptr<ProjectFile> interactableView(const std::string &path);
	static std::unique_ptr<ProjectFile> interfaceView(const std::string &path);
};

#endif
