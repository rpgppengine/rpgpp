#ifndef _EDITOR_UTILS_PROJECT_H
#define _EDITOR_UTILS_PROJECT_H

#include <memory>
#include <string>

#include "edui/signal.hpp"
#include "utils/subprocess.hpp"
#include "widgets/windows/gameOutput.hpp"

namespace editor {
class Project {
public:
	edui::Signal projectOpened;

	Project();
	void update();

	void openFromPath(const std::string &path);
	void create(const std::string &title, const std::string &dirPath);
	void spawnGame();

	Subprocess &getProcess();

	void setTitle(const std::string &title);
	std::string getTitle();

private:
	std::string title = "";
	std::string basePath = "";

	Subprocess process;
	std::shared_ptr<GameOutput> outputWindow = nullptr;
	bool outputOpen = false;
};
}  // namespace editor

#endif
