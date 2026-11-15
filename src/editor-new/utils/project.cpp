#include "utils/project.hpp"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <ostream>
#include <sstream>
#include <system_error>

#include "bins.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/details/helpers.hpp"
#include "editor.hpp"
#include "edui/gui.hpp"
#include "gamedata.hpp"
#include "packHelper.hpp"
#include "raylib.h"
#include "resource.hpp"
#include "rres.h"
#include "tileset.hpp"
#include "utils/resourcePacker.hpp"
#include "utils/resourceTypeManager.hpp"
#include "utils/subprocess.hpp"
#include "widgets/windows/gameOutput.hpp"

using namespace editor;

Project::Project() {}

void Project::update() { process.update(); }

void Project::openFromPath(const std::string &path) {
	if (!FileExists(path.c_str())) {
		Editor::getInstance().getInterface().showErrorMessage("File does not exist.");
		return;
	}

	std::ifstream inStream(path);
	{
		cereal::JSONInputArchive ar(inStream);
		ar(*this);
	}

	fileName = GetFileName(path.c_str());
	basePath = GetDirectoryPath(path.c_str());
	auto recent = Editor::getInstance().getResources().getRecentProjects();
	recent.enqueue(path);

	ChangeDirectory(basePath.c_str());

	projectOpened.invoke();
}

void Project::create(const std::string &title, const std::string &dirPath) {
	this->title = title;

	std::filesystem::path stdPath = dirPath;
	stdPath /= title;

	if (DirectoryExists(stdPath.c_str())) {
		Editor::getInstance().getInterface().showErrorMessage("Directory already exists.");
		return;
	}

	MakeDirectory(stdPath.c_str());
	basePath = stdPath;

	std::filesystem::path iniPath = stdPath / "proj.rpgpp";
	std::ofstream outStream(iniPath);

	{
		cereal::JSONOutputArchive ar(outStream);
		ar(*this);
	}

	std::filesystem::path defaultsDir = GetWorkingDirectory();
	defaultsDir /= "resources";
	defaultsDir /= "defaults";
	for (auto dirEntry : std::filesystem::recursive_directory_iterator(defaultsDir)) {
		if (std::filesystem::is_directory(dirEntry)) {
			std::filesystem::copy(dirEntry, stdPath / GetFileName(dirEntry.path().c_str()),
								  std::filesystem::copy_options::recursive);
		}
	}

	openFromPath(iniPath);
}

void Project::saveProject() {
	std::filesystem::path fullPath = basePath;
	fullPath /= fileName;

	std::ofstream outStream(fullPath);
	{
		cereal::JSONOutputArchive ar(outStream);
		ar(*this);
	}
}

#include <cstdio>

void Project::exportBin() {
	std::filesystem::path binPath = basePath;
	binPath /= "game.bin";

	GameData data;
	data.title = title;
	data.gameSet = gameSet;
	data.programSet = programSet;

	MinData inData;
	inData.title = title;
	inData.gameSet = gameSet;
	inData.programSet = programSet;

	// pack
	ResourcePacker packer;
	packer.setBasePath(basePath);
	packer.setMinData(inData);
	packer.pack(binPath);

	// copy over interpreter executable
	auto interpreterPath = Editor::getInstance().getResources().getInterpreterPath();

	std::filesystem::path destInterpreter = basePath;
	destInterpreter /= title;

	std::error_code ec;
	std::filesystem::copy(interpreterPath, destInterpreter, std::filesystem::copy_options::overwrite_existing, ec);
	if (ec.value() != 0) {
		Editor::getInstance().getInterface().showErrorMessage(ec.message());
	}

	//
	std::string search = "@GAMEDATA";
	uint32_t testId =
		rresComputeCRC32(reinterpret_cast<const unsigned char *>(search.data()), static_cast<int>(search.size()));
	auto chunk = rresLoadResourceChunk("game.rres", testId);

	size_t size = chunk.data.props[0];
	auto dataRaw = chunk.data.raw;

	std::string bytes = static_cast<char *>(dataRaw);
	MinData gameData;
	std::stringstream in;
	in.write(static_cast<const char *>(dataRaw), size);
	{
		cereal::PortableBinaryInputArchive ar(in);
		ar(gameData);
	}

	printf("%s \n", gameData.title.c_str());

	rresUnloadResourceChunk(chunk);
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
	}
	outputWindow->clear();

	outputWindow->centerLayout();
	outputWindow->onDeleted.connect([this] { outputOpen = false; });
	Editor::getInstance().getInterface().getEdui().add(outputWindow, edui::EDUI_MAX_LAYERS - 1);
}

std::string Project::getTitle() { return title; }

void Project::setTitle(const std::string &title) { this->title = title; }

std::string Project::getBasePath() { return basePath; }

Subprocess &Project::getProcess() { return process; }
