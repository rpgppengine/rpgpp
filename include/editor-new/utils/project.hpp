#ifndef _EDITOR_UTILS_PROJECT_H
#define _EDITOR_UTILS_PROJECT_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

#include "cereal/cereal.hpp"
#include "edui/signal.hpp"
#include "gamedata.hpp"
#include "rres.h"
#include "utils/subprocess.hpp"
#include "widgets/windows/gameOutput.hpp"

namespace editor {
class Project {
public:
	edui::Signal projectOpened;

	ProjectProgramSettings programSet;
	ProjectGameSettings gameSet;

	Project();
	void update();

	void openFromPath(const std::string &path);
	void create(const std::string &title, const std::string &dirPath);
	void saveProject();
	void exportBin();
	void spawnGame();

	Subprocess &getProcess();

	void setTitle(const std::string &title);
	std::string getTitle();
	std::string getBasePath();

private:
	std::string title = "";
	std::string basePath = "";
	std::string fileName = "";

	Subprocess process;
	std::shared_ptr<GameOutput> outputWindow = nullptr;
	bool outputOpen = false;

public:
	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(title), CEREAL_NVP(programSet), CEREAL_NVP(gameSet));
	}
};
}  // namespace editor

#endif
