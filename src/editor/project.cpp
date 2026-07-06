#include "project.hpp"

#include <raylib.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <pugixml.hpp>
#include <string>
#include <vector>

#include "conversion.hpp"
#include "dialogue.hpp"
#include "dialogueParser.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "interfaceView.hpp"
#include "room.hpp"
#include "screens/projectScreen.hpp"
#include "services/fileSystemService.hpp"
#include "tileset.hpp"

#ifdef _WIN32

#include <winapi.hpp>

#else

#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#endif

using json = nlohmann::json;

Project::Project(const std::string &path) {
	this->projectPath = GetDirectoryPath(path.c_str());

	char *jsonContent = LoadFileText(path.c_str());
	json j = json::parse(jsonContent);

	programSet.projectTitle = j.value("title", "");
	programSet.windowSize = {j.value("windowSize", json::array({640, 480}))[0],
							 j.value("windowSize", json::array({640, 480}))[1]};
	programSet.projectVersion = j.value("version", "");
	programSet.programIconPath = j.value("programIcon", "");
	programSet.windowResizeableFlag = j.value("windowResizeable", false);
	programSet.windowStateFlag = j.value("windowState", 0);
	programSet.targetFPS = j.value("targetFPS", 60);

	gameSet.defaultRoomPath = j.value("defaultRoom", "");
	gameSet.playerActorPath = j.value("playerActor", "");
	gameSet.tileSize = j.value("tileSize", 16);
	gameSet.debugDraw = j.value("debugDraw", false);
	gameSet.exportImageScales = j.value("exportImageScales", std::vector<int>{1});
	gameSet.exportFontSizes = j.value("exportFontSizes", std::vector<int>{13});

	ChangeDirectory(projectPath.c_str());
	UnloadFileText(jsonContent);
}

std::string Project::create(const std::string &dirPath, const std::string &title) {
	Project p;
	p.getProgramSettings().projectTitle = title;
	json j = p.toJson();
	std::string fileContent = j.dump();

	std::filesystem::path filePath = dirPath;
	filePath /= "proj.rpgpp";
	SaveFileText(filePath.u8string().c_str(), fileContent.c_str());

	for (int i = 0; i < FILETYPE_MAX; i++) {
		EngineFileType fileType = static_cast<EngineFileType>(i);

		if (fileType != EngineFileType::FILE_EMPTY) {
			std::string dirName = TextToLower(Editor::instance->getFs().getTypeName(fileType).c_str());

			std::string typeDirPath = TextFormat("%s/%s", dirPath.c_str(), dirName.c_str());

			std::string defaultDirName = TextFormat(
				"%s/resources/defaults/%s", Editor::instance->getFs().getEditorBaseDir().c_str(), dirName.c_str());

			if (DirectoryExists(defaultDirName.c_str())) {
				std::filesystem::copy(defaultDirName, typeDirPath, std::filesystem::copy_options::recursive);
			} else {
				MakeDirectory(std::filesystem::path(dirPath).append(dirName).u8string().c_str());
			}
		}
	}

	return filePath.u8string();
}

void Project::openProject(const tgui::String &filePath, bool forceSwitch) {
	Editor::instance->setProject(filePath.toStdString());
	Editor::instance->getRecentProjectService().enqueue(filePath.toStdString());
	Editor::instance->getGui().setScreen(std::make_unique<screens::ProjectScreen>(), forceSwitch);
}

json Project::toJson() {
	json j = json::object();

	j["title"] = programSet.projectTitle;
	j["windowSize"] = {programSet.windowSize.x, programSet.windowSize.y};
	j["version"] = programSet.projectVersion;
	j["programIcon"] = programSet.programIconPath;
	j["windowResizeable"] = programSet.windowResizeableFlag;
	j["windowState"] = programSet.windowStateFlag;
	j["targetFPS"] = programSet.targetFPS;

	j["defaultRoom"] = gameSet.defaultRoomPath;
	j["tileSize"] = gameSet.tileSize;
	j["playerActor"] = gameSet.playerActorPath;
	j["debugDraw"] = gameSet.debugDraw;
	j["exportImageScales"] = gameSet.exportImageScales;
	j["exportFontSizes"] = gameSet.exportFontSizes;

	return j;
}

ProjectProgramSettings &Project::getProgramSettings() { return programSet; }

ProjectGameSettings &Project::getGameSettings() { return gameSet; }

std::string &Project::getBasePath() { return projectPath; }

std::vector<std::string> Project::getPaths(EngineFileType fileType) {
	std::filesystem::path subdir = projectPath;
	subdir /= TextToLower(Editor::instance->getFs().getTypeName(fileType).c_str());

	auto &extensions = Editor::instance->getFs().getTypeExtensions(fileType);

	assert(subdir.string().empty() == false && "directory path is empty");

	auto pathList = LoadDirectoryFiles(subdir.string().c_str());
	std::vector<std::string> vec = {};

	for (int i = 0; i < pathList.count; i++) {
		std::string fullPath = pathList.paths[i];
		std::string fileExt = GetFileExtension(fullPath.c_str());
		for (auto &j : extensions) {
			if (j == fileExt) {
				vec.emplace_back(fullPath);
				break;
			}
		}
	}

	UnloadDirectoryFiles(pathList);

	return vec;
}

std::string Project::getResourcePath(EngineFileType fileType, const std::string &fileName) {
	std::filesystem::path subdir = projectPath;
	subdir /= TextToLower(Editor::instance->getFs().getTypeName(fileType).c_str());
	subdir /= fileName;

	return subdir.string();
}

std::map<std::string, std::string> Project::getInteractableNames() {
	std::map<std::string, std::string> map{};

	// built-in interactables
	std::filesystem::path builtinInteractablesDir = Editor::instance->getFs().getEditorBaseDir();
	builtinInteractablesDir /= "resources";
	builtinInteractablesDir /= "interactables";

	auto builtinList = LoadDirectoryFiles(builtinInteractablesDir.u8string().c_str());
	for (int i = 0; i < builtinList.count; i++) {
		std::string intPath = builtinList.paths[i];
		Interactable inter(intPath);

		map[intPath.c_str()] = inter.getDisplayTitle();
	}
	UnloadDirectoryFiles(builtinList);

	auto userList = LoadDirectoryFiles("interactables/");
	for (int i = 0; i < userList.count; i++) {
		std::string intPath = userList.paths[i];
		Interactable inter(intPath);

		map[intPath.c_str()] = inter.getDisplayTitle();
	}

	UnloadDirectoryFiles(userList);

	return map;
}

std::vector<std::string> Project::getPropsNames() {
	std::vector<std::string> vec;
	for (auto propPath : getPaths(EngineFileType::FILE_PROP)) {
		vec.push_back(propPath);
	}
	return vec;
}

GameData Project::generateStruct() {
	GameData data;
	data.title = programSet.projectTitle;
	data.programSet = programSet;
	data.gameSet = gameSet;

	for (auto tileSetPath : getPaths(EngineFileType::FILE_TILESET)) {
		TileSet tileSet(tileSetPath);
		Texture texture = tileSet.getTexture();
		Image image = LoadImageFromTexture(texture);

		int fileSize = 0;
		std::string fileType = GetFileExtension(tileSet.getTextureSource().c_str());
		unsigned char *imageData = ExportImageToMemory(image, fileType.c_str(), &fileSize);

		TileSetBin tileSetBin;
		tileSetBin.name = GetFileName(tileSetPath.c_str());
		tileSetBin.extension = GetFileExtension(tileSet.getTextureSource().c_str());
		for (int i = 0; i < fileSize; i++) {
			tileSetBin.image.push_back(*imageData);
			imageData++;
		}
		tileSetBin.tileSize =
			IVector{static_cast<int>(tileSet.getTileSize().x), static_cast<int>(tileSet.getTileSize().y)};
		tileSetBin.dataSize = fileSize;

		data.tilesets[GetFileName(tileSetPath.c_str())] = tileSetBin;

		UnloadImage(image);
	}

	for (auto roomPath : getPaths(EngineFileType::FILE_MAP)) {
		std::unique_ptr<TileMap> map = std::make_unique<TileMap>(roomPath);

		RoomBin roomBin;
		roomBin.name = GetFileNameWithoutExt(roomPath.c_str());
		roomBin.tileSetName = GetFileName(map->getTileSetSource().c_str());
		Vector2 worldSize = map->getMaxWorldSize();
		roomBin.width = static_cast<int>(worldSize.x);
		roomBin.height = static_cast<int>(worldSize.y);

		for (int x = 0; x < roomBin.width; x++) {
			std::vector<TileBin> row;
			for (int y = 0; y < roomBin.height; y++) {
				TileBin tile;
				row.push_back(tile);
			}
			roomBin.tiles.push_back(row);
		}
		for (int x = 0; x < roomBin.width; x++) {
			for (int y = 0; y < roomBin.height; y++) {
				Tile tile = map->getTile(x, y);
				Vector2 atlasPos = tile.getAtlasTile().getAtlasCoords();
				Vector2 worldPos = tile.getWorldCoords();

				IVector intAtlas = IVector{static_cast<int>(atlasPos.x), static_cast<int>(atlasPos.y)};
				IVector intWorld = IVector{static_cast<int>(worldPos.x), static_cast<int>(worldPos.y)};

				TileBin tileBin;
				tileBin.atlasPos = intAtlas;
				tileBin.worldPos = intWorld;
				roomBin.tiles[x][y] = tileBin;
			}
		}

		map.reset();

		std::unique_ptr<Room> room = std::make_unique<Room>(roomPath);
		roomBin.startPoint =
			IVector{static_cast<int>(room->getStartTile().x), static_cast<int>(room->getStartTile().y)};
		for (auto [pos, obj] : room->getCollisions().getObjects()) {
			IVector intVec;
			intVec.x = static_cast<int>(pos.x);
			intVec.y = static_cast<int>(pos.y);
			roomBin.collisions.push_back(intVec);
		}
		for (auto interactable : room->getInteractables().getList()) {
			InteractableInRoomBin intBin;
			intBin.x = static_cast<int>(interactable->getWorldPos().x);
			intBin.y = static_cast<int>(interactable->getWorldPos().y);
			intBin.type = interactable->getType();
			intBin.onTouch = interactable->isOnTouch();

			// add missing props to the interactable in the map
			for (auto &item : getInteractableNames()) {
				std::string itemType = GetFileNameWithoutExt(item.first.c_str());
				if (itemType == interactable->getType()) {
					Interactable itemInteractable(item.first);

					for (auto prop : itemInteractable.getProps().items()) {
						if (!interactable->getProps().contains(prop.key())) {
							interactable->getProps().push_back({prop.key(), prop.value()});
						}
					}

					break;
				}
			}

			intBin.propsCbor = nlohmann::json::to_cbor(interactable->getProps());

			roomBin.interactables.push_back(intBin);
		}
		for (auto &[pos, prop] : room->getProps().getObjects()) {
			PropInRoomBin pBin;
			pBin.name = prop->getSourcePath();
			pBin.tilePos = fromVector2(prop->getWorldTilePos());

			auto *interactable = prop->getInteractable();

			// add missing props to the interactable in the prop
			for (auto &item : getInteractableNames()) {
				std::string itemType = GetFileNameWithoutExt(item.first.c_str());
				if (itemType == interactable->getType()) {
					Interactable itemInteractable(item.first);

					for (auto prop : itemInteractable.getProps().items()) {
						if (!interactable->getProps().contains(prop.key())) {
							interactable->getProps().push_back({prop.key(), prop.value()});
						}
					}

					break;
				}
			}

			pBin.propsCbor = nlohmann::json::to_cbor(prop->getInteractable()->getProps());

			roomBin.props.push_back(pBin);
		}
		for (auto &[aName, actor] : room->getActors().getActors()) {
			ActorInRoomBin aBin;
			aBin.name = aName;
			aBin.source = actor->getSourcePath();
			aBin.tilePos =
				IVector{static_cast<int>(actor->getTilePosition().x), static_cast<int>(actor->getTilePosition().y)};

			if (actor->hasInteractable()) {
				aBin.intType = actor->getInteractable()->getType();
				aBin.propsCbor = nlohmann::json::to_cbor(actor->getInteractable()->getProps());
			} else {
				aBin.intType = "";
			}
			roomBin.actors.push_back(aBin);
		}
		roomBin.musicSource = room->getMusicSource();

		room.reset();
		data.rooms.push_back(roomBin);
	}

	for (auto actorPath : getPaths(EngineFileType::FILE_ACTOR)) {
		std::unique_ptr<Actor> actor = std::make_unique<Actor>(actorPath);

		ActorBin actorBin;
		actorBin.name = GetFileName(actorPath.c_str());
		actorBin.tileSetName = GetFileName(actor->getTileSetSource().c_str());

		Rectangle collisionRect = actor->getCollisionRect();
		actorBin.collision = IRect{static_cast<int>(collisionRect.x), static_cast<int>(collisionRect.y),
								   static_cast<int>(collisionRect.width), static_cast<int>(collisionRect.height)};
		std::array<std::vector<Vector2>, 8> animations = actor->getAnimationsRaw();
		for (int i = 0; i < 8; i++) {
			for (int frameIndex = 0; frameIndex < animations[i].size(); frameIndex++) {
				Vector2 vec = animations[i][frameIndex];
				IVector intVec = IVector{static_cast<int>(vec.x), static_cast<int>(vec.y)};

				actorBin.animations[i].push_back(intVec);
			}
		}

		data.actors[GetFileNameWithoutExt(actorPath.c_str())] = actorBin;
	}

	for (auto diagPath : getPaths(EngineFileType::FILE_DIALOGUE)) {
		Dialogue dialogue(diagPath);
		DialogueBin diag = dialogue.getData();

		diag.title = GetFileNameWithoutExt(diagPath.c_str());
		for (auto &line : diag.lines) {
			for (auto &option : line.options) {
				option.nextDialogue = GetFileNameWithoutExt(option.nextDialogue.c_str());
			}
		}
		data.dialogues[GetFileNameWithoutExt(diagPath.c_str())] = diag;
	}

	for (auto imagePath : getPaths(EngineFileType::FILE_IMAGE)) {
		Image img = LoadImage(imagePath.c_str());
		ImageBin bin;
		bin.ext = GetFileExtension(imagePath.c_str());

		int fileSize = 0;

		unsigned char *imgData = ExportImageToMemory(img, GetFileExtension(imagePath.c_str()), &fileSize);
		for (int i = 0; i < fileSize; i++) {
			bin.data.push_back(*imgData);
			imgData++;
		}
		bin.dataSize = fileSize;

		UnloadImage(img);

		data.images[GetFileName(imagePath.c_str())] = bin;
	}

	for (auto fontPath : getPaths(EngineFileType::FILE_FONT)) {
		FontBin fontBin;

		int dataSize = 0;
		auto fileData = LoadFileData(fontPath.c_str(), &dataSize);

		for (int i = 0; i < dataSize; i++) {
			fontBin.data.push_back(fileData[i]);
		}

		fontBin.ext = GetFileExtension(fontPath.c_str());
		fontBin.dataSize = dataSize;
		data.fonts[GetFileNameWithoutExt(fontPath.c_str())] = fontBin;

		UnloadFileData(fileData);
	}

	for (auto soundPath : getPaths(EngineFileType::FILE_SOUND)) {
		MusicBin soundBin;

		int dataSize = 0;
		auto fileData = LoadFileData(soundPath.c_str(), &dataSize);

		for (int i = 0; i < dataSize; i++) {
			soundBin.fileData.push_back(fileData[i]);
		}

		soundBin.fileExt = GetFileExtension(soundPath.c_str());
		soundBin.isSound = true;
		data.music[GetFileNameWithoutExt(soundPath.c_str())] = soundBin;

		UnloadFileData(fileData);
	}

	for (auto musicPath : getPaths(EngineFileType::FILE_MUSIC)) {
		MusicBin musicBin;

		int dataSize = 0;
		auto fileData = LoadFileData(musicPath.c_str(), &dataSize);

		for (int i = 0; i < dataSize; i++) {
			musicBin.fileData.push_back(fileData[i]);
		}

		musicBin.fileExt = GetFileExtension(musicPath.c_str());
		musicBin.isSound = false;
		data.music[GetFileNameWithoutExt(musicPath.c_str())] = musicBin;

		UnloadFileData(fileData);
	}

	for (auto propPath : getPaths(EngineFileType::FILE_PROP)) {
		Prop prop = Prop(propPath);

		PropBin bin;
		bin.name = GetFileNameWithoutExt(propPath.c_str());
		bin.atlasRect =
			IRect{static_cast<int>(prop.getAtlasRect().x), static_cast<int>(prop.getAtlasRect().y),
				  static_cast<int>(prop.getAtlasRect().width), static_cast<int>(prop.getAtlasRect().height)};
		bin.collisionRect =
			IRect{static_cast<int>(prop.getCollisionRect().x), static_cast<int>(prop.getCollisionRect().y),
				  static_cast<int>(prop.getCollisionRect().width), static_cast<int>(prop.getCollisionRect().height)};
		bin.imagePath = std::string(prop.getImagePath());
		bin.hasInteractable = prop.getHasInteractable();
		if (prop.getInteractable() == nullptr) {
			bin.intType = "";
		} else {
			bin.intType = prop.getInteractable()->getType();
		}

		data.props.push_back(bin);
	}

	// built in interactables
	std::filesystem::path interactablesDir = Editor::instance->getFs().getEditorBaseDir();
	interactablesDir /= "resources";
	interactablesDir /= "interactables";

	auto list = LoadDirectoryFiles(interactablesDir.u8string().c_str());
	for (int i = 0; i < list.count; i++) {
		std::string intPath = list.paths[i];
		Interactable inter(intPath);

		InteractableBin bin;
		bin.typeName = inter.getType();
		bin.scriptPath = inter.getScriptSourcePath();
		bin.props = nlohmann::json::to_cbor(inter.getProps());

		data.interactables[inter.getType()] = bin;
	}
	UnloadDirectoryFiles(list);

	// user interactables
	auto userList = LoadDirectoryFiles("interactables/");
	for (int i = 0; i < userList.count; i++) {
		std::string intPath = userList.paths[i];
		Interactable inter(intPath);

		InteractableBin bin;
		bin.typeName = inter.getType();
		bin.scriptPath = inter.getScriptSourcePath();
		bin.props = nlohmann::json::to_cbor(inter.getProps());

		data.interactables[inter.getType()] = bin;
	}

	UnloadDirectoryFiles(userList);

	// built-in scripts
	std::filesystem::path scriptsDir = Editor::instance->getFs().getEditorBaseDir();
	scriptsDir /= "resources";
	scriptsDir /= "scripts";
	auto scriptsList = LoadDirectoryFiles(scriptsDir.u8string().c_str());
	for (int i = 0; i < scriptsList.count; i++) {
		std::string scriptPath = scriptsList.paths[i];
		auto scriptText = LoadFileText(scriptPath.c_str());

		/*
		sol::state lua;
		auto lr = lua.load(scriptText);

		SOL_ASSERT(lr.valid());

		sol::protected_function target = lr.get<sol::protected_function>();
		auto bytecode = target.dump();
		auto sv = bytecode.as_string_view();
		*/

		ScriptBin bin;
		// bin.bytecode = std::string(sv.data());
		bin.bytecode = scriptText;

		data.scripts[GetFileName(scriptPath.c_str())] = bin;
		UnloadFileText(scriptText);
	}
	UnloadDirectoryFiles(scriptsList);

	// user scripts
	auto userScriptsList = LoadDirectoryFiles("scripts/");
	for (int i = 0; i < userScriptsList.count; i++) {
		std::string scriptPath = userScriptsList.paths[i];
		auto scriptText = LoadFileText(scriptPath.c_str());

		ScriptBin bin;
		bin.bytecode = scriptText;

		data.scripts[TextFormat("scripts/%s", GetFileName(scriptPath.c_str()))] = bin;
		UnloadFileText(scriptText);
	}

	for (auto viewPath : getPaths(EngineFileType::FILE_INTERFACEVIEW)) {
		InterfaceView view(viewPath);

		InterfaceViewBin bin;

		for (ElementIndex i = 0; i < MAX_ELEMENTS; i++) {
			auto element = view.getElement(i);

			if (element != nullptr) {
				UIElementBin elementBin;
				elementBin.props = element->props;
				elementBin.type = element->typeName;

				bin.elements[view.getEntityName(i)] = elementBin;
			}
		}

		bin.scriptSource = view.getScriptFile();

		data.interfaceViews[GetFileNameWithoutExt(viewPath.c_str())] = bin;
	}

	return data;
}

void Project::runProject() {
	// Generate the bin first
	auto bin = generateStruct();

	std::filesystem::path binPath = projectPath;
	binPath /= "game.bin";
	serializeDataToFile(binPath.u8string(), bin);

	// run project
	std::string editorBasePath = Editor::instance->getFs().getEditorBaseDir();

	std::filesystem::path libPath = editorBasePath;
	libPath /= "game-src";
	libPath /= "lib";
	std::filesystem::path libDest = projectPath;
	std::filesystem::path scriptPath = editorBasePath;
	scriptPath /= "resources/scripts/script.lua";

	std::filesystem::path intepreterPath = editorBasePath;

	intepreterPath /= "execs";

#ifdef __linux
	libPath /= "librpgpplua.so";
	intepreterPath /= "luajit";
	libDest /= "rpgpplua.so";

	std::filesystem::copy_file(libPath, libDest, std::filesystem::copy_options::overwrite_existing);

	ChangeDirectory(projectPath.c_str());

	std::string cmdLine = TextFormat("%s -l rpgpplua %s", intepreterPath.c_str(), scriptPath.c_str());
	printf("%s \n", cmdLine.c_str());

	char buffer[256];
	FILE *stream;
	stream = popen(TextFormat("%s -l rpgpplua %s", intepreterPath.c_str(), scriptPath.c_str()), "r");

#endif
#ifdef _WIN32
	intepreterPath /= "luajit.exe";

	const std::filesystem::path rpgppDllPath = "rpgpplua.dll";
	libPath /= rpgppDllPath;
	libDest /= rpgppDllPath;

	std::filesystem::copy_file(libPath, libDest, std::filesystem::copy_options::overwrite_existing);

	ChangeDirectory(projectPath.c_str());

	// note: compared to linux, you have to add .string() to every single one of
	// these paths.
	std::string cmdLine = TextFormat("%s -l rpgpplua %s", intepreterPath.string().c_str(), scriptPath.string().c_str());

	WinRunWithLog("playtest.log", cmdLine);

#endif
}

void Project::buildProject() {
	// Generate the bin file first
	auto bin = generateStruct();

	std::filesystem::path binPath = projectPath;
	binPath /= "game.bin";
	serializeDataToFile(binPath.u8string(), bin);

	// Copy base game file
	std::filesystem::path baseGamePath = Editor::instance->getFs().getEditorBaseDir();

	std::filesystem::path resultPath = projectPath;

#ifdef _WIN32
	baseGamePath /= "game.exe";
	resultPath /= TextFormat("%s.exe", programSet.projectTitle.c_str());
#else
	baseGamePath /= "game";
	resultPath /= programSet.projectTitle;
#endif

	try {
		std::filesystem::copy(baseGamePath, resultPath, std::filesystem::copy_options::overwrite_existing);
	} catch (const std::exception &) {
		printf("failed to copy file, aborting...\n");
		return;
	}

#ifdef _WIN32

	WinRunWithLog("build.log", resultPath.string().data());

#else

	pid_t pid = fork();

	if (pid == 0) {
		int fd = open("log.log", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

		dup2(fd, STDOUT_FILENO);

		execl(resultPath.c_str(), "");
	} else if (pid > 0) {
		printf("Started the game.. \n");
		wait(0);
	} else {
		printf("Error while forking..\n");
	}

#endif
}
