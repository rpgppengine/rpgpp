#include "fileInitVisitor.hpp"

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <utility>

#include "TGUI/Widget.hpp"
#include "actor.hpp"
#include "dialogue.hpp"
#include "editor.hpp"
#include "gamedata.hpp"
#include "interactable.hpp"
#include "interfaceView.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "room.hpp"
#include "screens/projectScreen.hpp"
#include "services/fileSystemService.hpp"
#include "tilemap.hpp"
#include "tileset.hpp"
#include "widgets/newFileDialog.hpp"

FileInitVisitor::FileInitVisitor() {
	funcs[static_cast<int>(EngineFileType::FILE_TILESET)] = tileset;
	funcs[static_cast<int>(EngineFileType::FILE_MAP)] = room;
	funcs[static_cast<int>(EngineFileType::FILE_ACTOR)] = actor;
	funcs[static_cast<int>(EngineFileType::FILE_PROP)] = prop;
	funcs[static_cast<int>(EngineFileType::FILE_DIALOGUE)] = dialogue;
	funcs[static_cast<int>(EngineFileType::FILE_INTERACTABLE)] = interactable;
	funcs[static_cast<int>(EngineFileType::FILE_INTERFACEVIEW)] = interface;
}

bool FileInitVisitor::funcIsEmpty(EngineFileType fileType) { return funcs[static_cast<int>(fileType)] == nullptr; }

void FileInitVisitor::visit(EngineFileType fileType, NewFileDialog::Ptr dialog) {
	if (funcIsEmpty(fileType)) {
		empty(dialog);
	} else {
		funcs[static_cast<int>(fileType)](dialog);
	}
}

void createDirectoryIfNotExist(const char* directoryName) {
	if (!DirectoryExists(directoryName))
		MakeDirectory(directoryName);
}


void FileInitVisitor::empty(NewFileDialog::Ptr dialog) {
	dialog->confirmButton->onPress([dialog] {
		printf("%s \n", dialog->titleField->getText().toStdString().c_str());
		printf("%s \n", dialog->fileField->getChosenPath().toStdString().c_str());
		dialog->window->close();
	});
}

void FileInitVisitor::tileset(NewFileDialog::Ptr dialog) {
	dialog->fileLabel->setText("Texture");
	dialog->setPathFilters({{"Images", {"*.png", "*.jpg"}}});
	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		createDirectoryIfNotExist("images");
		filePath = TextFormat("images/%s", GetFileName(filePath.c_str()));
		if (!title.empty() && !filePath.empty()) {
			dialog->window->close();

			std::unique_ptr<TileSet> tileSet = std::make_unique<TileSet>(filePath, 16);
			nlohmann::json fileJson = tileSet->dumpJson();
			std::string newFilePath = TextFormat("tilesets/%s.rtiles", title.c_str());
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_TILESET, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_TILESET);
		}
	});
}

void FileInitVisitor::room(NewFileDialog::Ptr dialog) {
	dialog->fileLabel->setText("TileSet");
	dialog->setPathFilters({{"RPG++ TileSet", {"*.rtiles"}}});
	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		createDirectoryIfNotExist("tilesets");
		std::string shortFilePath = TextFormat("tilesets/%s", GetFileName(filePath.c_str()));
		if (!title.empty() && !filePath.empty()) {
			std::unique_ptr<TileSet> tileSet = std::make_unique<TileSet>(shortFilePath);
			std::unique_ptr<TileMap> tileMap = std::make_unique<TileMap>(std::move(tileSet), 20, 20, _RPGPP_TILESIZE,
																		 _RPGPP_TILESIZE * RPGPP_DRAW_MULTIPLIER);
			std::unique_ptr<Room> room = std::make_unique<Room>(std::move(tileMap));

			std::string newFilePath = TextFormat("maps/%s.rmap", title.c_str());
			nlohmann::json fileJson = room->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_MAP, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_MAP);

			dialog->window->close();
		}
	});
}

void FileInitVisitor::actor(NewFileDialog::Ptr dialog) {
	dialog->fileLabel->setText("TileSet");
	dialog->setPathFilters({{"RPG++ TileSet", {"*.rtiles"}}});

	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		createDirectoryIfNotExist("tilesets");
		filePath = TextFormat("tilesets/%s", GetFileName(filePath.c_str()));
		if (!title.empty() && !filePath.empty()) {
			std::unique_ptr<TileSet> tileSet = std::make_unique<TileSet>(filePath);
			std::string relativeTileSetSource = TextFormat("tilesets/%s", GetFileName(filePath.c_str()));
			std::unique_ptr<Actor> actor =
				std::make_unique<Actor>(std::move(tileSet), Vector2{0, 0}, relativeTileSetSource);

			std::string newFilePath = TextFormat("actors/%s.ractor", title.c_str());
			nlohmann::json fileJson = actor->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_ACTOR, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_ACTOR);

			dialog->window->close();
		}
	});
}

void FileInitVisitor::prop(NewFileDialog::Ptr dialog) {
	dialog->fileLabel->setText("Image");
	dialog->setPathFilters({{"Image", {"*.png", "*.jpg"}}});

	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		createDirectoryIfNotExist("images");
		filePath = TextFormat("images/%s", GetFileName(filePath.c_str()));
		if (!title.empty() && !filePath.empty()) {
			std::unique_ptr<Prop> prop = std::make_unique<Prop>(Rectangle{0, 0, 16, 16}, Vector2{0, 0});
			prop->setTextureFromPath(TextFormat("images/%s", GetFileName(filePath.c_str())));

			std::string newFilePath = TextFormat("props/%s.rprop", title.c_str());
			nlohmann::json fileJson = prop->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_PROP, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_PROP);

			dialog->window->close();
		}
	});
}

void FileInitVisitor::dialogue(NewFileDialog::Ptr dialog) {
	dialog->hideFileField();

	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();
		std::string filePath = dialog->fileField->getChosenPath().toStdString();
		createDirectoryIfNotExist("images");
		filePath = TextFormat("images/%s", GetFileName(filePath.c_str()));
		if (!title.empty()) {
			std::unique_ptr<Dialogue> diag = std::make_unique<Dialogue>();

			std::string newFilePath = TextFormat("dialogues/%s.rdiag", title.c_str());
			nlohmann::json fileJson = diag->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_DIALOGUE, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_DIALOGUE);

			dialog->window->close();
		}
	});
}

void FileInitVisitor::interactable(NewFileDialog::Ptr dialog) {
	dialog->hideFileField();

	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();

		bool found = false;
		for (auto &[key, val] : Editor::instance->getProject()->getInteractableNames()) {
			std::string type = GetFileNameWithoutExt(key.c_str());

			if (type == std::string(TextToLower(title.c_str()))) {
				found = true;
				break;
			}
		}

		if (!title.empty() && !found) {
			std::unique_ptr<Interactable> interactable =
				std::make_unique<Interactable>(title, Vector2{-1, -1}, _RPGPP_TILESIZE);
			interactable->setDisplayTitle(title);
			createDirectoryIfNotExist("interactibles");
			std::string newFilePath = TextFormat("interactables/%s.rinter", TextToLower(title.c_str()));
			nlohmann::json fileJson = interactable->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_INTERACTABLE, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_INTERACTABLE);

			dialog->window->close();
		}
	});
}

void FileInitVisitor::interface(NewFileDialog::Ptr dialog) {
	dialog->hideFileField();

	dialog->confirmButton->onPress([dialog] {
		std::string title = dialog->titleField->getText().toStdString();

		if (!title.empty()) {
			std::unique_ptr<InterfaceView> interfaceView = std::make_unique<InterfaceView>();
			createDirectoryIfNotExist("views");
			std::string newFilePath = TextFormat("views/%s.rui", title.c_str());
			nlohmann::json fileJson = interfaceView->dumpJson();
			SaveFileText(newFilePath.c_str(), fileJson.dump().c_str());

			auto ptr = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
			ptr->addFileView(EngineFileType::FILE_INTERFACEVIEW, newFilePath);
			ptr->addResourceButtons(EngineFileType::FILE_INTERFACEVIEW);

			dialog->window->close();
		}
	});
}
