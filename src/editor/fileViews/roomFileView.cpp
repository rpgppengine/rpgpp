#include "fileViews/roomFileView.hpp"

#include <memory>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Scrollbar.hpp"
#include "bindTranslation.hpp"
#include "editor.hpp"
#include "enum_visitor/enum_visitor.hpp"
#include "fileViews/fileView.hpp"
#include "raylib.h"
#include "room.hpp"
#include "roomViewModesHandler.hpp"
#include "views/roomView.hpp"
#include "views/tileSetView.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/toolbox.hpp"

RoomFileView::RoomFileView() {
	RoomTool a;
	TranslationService &ts = Editor::instance->getTranslations();
	HotkeyService &hks = Editor::instance->getHotkeyService();

	roomView = RoomView::create();
	roomView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), TextFormat("100%% - %d", TOOLBOX_H)});
	roomView->setPosition(0, TOOLBOX_H);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(roomView));
	widgetContainer.push_back(roomView);

	auto roomLayerGroup = tgui::Group::create();
	roomLayerGroup->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W), LAYER_CHOOSE_H);
	roomLayerGroup->setSize({ROOM_LAYER_W, ROOM_LAYER_H});

	layerVisitor.group = roomLayerGroup;
	roomView->layerVisitor = &layerVisitor;

	tileSetView = layerVisitor.tileSetView;
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(tileSetView));
	roomLayerGroup->add(tileSetView);
	widgetContainer.push_back(roomLayerGroup);

	roomView->tileSetView = tileSetView.get();
	roomView->interactableChoose = layerVisitor.interactableChoose.get();
	roomView->propChoose = layerVisitor.propChoose.get();

	modesHandler = std::make_unique<RoomViewModesHandler>();
	modesHandler->view = roomView;
	roomView->fileView = dynamic_cast<FileView *>(this);

	layerChoose = tgui::ComboBox::create();
	layerChoose->setPosition(TextFormat("100%% - %d", RIGHT_PANEL_W), 0);
	layerChoose->setSize(RIGHT_PANEL_W, LAYER_CHOOSE_H);
	layerChoose->addItem("Tiles");
	layerChoose->addItem("Collisions");
	layerChoose->addItem("Interactables");
	layerChoose->addItem("Props");
	layerChoose->addItem("Actors");
	layerChoose->setSelectedItemByIndex(0);
	widgetContainer.push_back(layerChoose);

	layerChoose->onItemSelect([this, roomLayerGroup](int index) {
		auto layerEnum = static_cast<RoomLayer>(index);
		roomView->setLayer(layerEnum);

		roomLayerGroup->removeAllWidgets();
		mj::visit(layerVisitor, layerEnum);
	});

	auto props = PropertiesBox::create();
	props->setSize({RIGHT_PANEL_W, "100%"});
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), ROOM_LAYER_H + LAYER_CHOOSE_H});

	widthField = IntField::create();
	bindTranslation(widthField->label, "screen.project.roomview.mapwidth", &tgui::Label::setText);
	widthField->value->onValueChange([this](const auto &value) {
		Vector2 worldSize = this->roomView->getRoom()->getTileMap()->getMaxWorldSize();
		worldSize.x = static_cast<int>(value);
		this->roomView->getRoom()->getTileMap()->setWorldSize(worldSize);
		this->dirty = true;
	});
	props->addIntField(widthField);

	heightField = IntField::create();
	bindTranslation(heightField->label, "screen.project.roomview.mapheight", &tgui::Label::setText);
	heightField->value->onValueChange([this](const float &value) {
		Vector2 worldSize = this->roomView->getRoom()->getTileMap()->getMaxWorldSize();
		worldSize.y = static_cast<int>(value);
		this->roomView->getRoom()->getTileMap()->setWorldSize(worldSize);
		this->dirty = true;
	});
	props->addIntField(heightField);

	tileSetField = FileField::create("", "...");
	bindTranslation(tileSetField->label, "screen.project.roomview.tileset_file", &tgui::Label::setText);
	tileSetField->pathFilters = {{"RPG++ TileSet", {"*.rtiles"}}};
	tileSetField->callback = [this](const tgui::String &path) {
		auto room = this->roomView->getRoom();
		room->getTileMap()->setTileSet(path.toStdString());
		tileSetView->setTileSet(room->getTileMap()->getTileSet());
		this->dirty = true;
	};
	props->addFileField(tileSetField);

	musicFileField = FileField::create("", "");
	bindTranslation(musicFileField->label, "screen.project.roomview.bg_music_file", &tgui::Label::setText);
	musicFileField->setWidgetName("file");
	musicFileField->pathFilters = {{"Music File", {"*.mp4", "*.ogg", "*.wav"}}};
	musicFileField->callback = [this](const tgui::String &path) {
		roomView->getRoom()->setMusicSource(GetFileNameWithoutExt(path.toStdString().c_str()));
		this->dirty = true;
	};

	props->addFileField(musicFileField);

	props->addButton("Clear Music", [this] {
		roomView->getRoom()->setMusicSource("");
		musicFileField->value->setText("");
		this->dirty = true;
	});

	widgetContainer.push_back(props);

	auto toolbox = Toolbox<RoomTool>::create();
	toolbox->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
	toolbox->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
	toolbox->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), TOOLBOX_H});

	std::vector<std::pair<std::string, ToolboxItem<RoomTool>>> tools = {
		{"room_tool.mouse", ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_NONE, "Mouse", "tool_none.png"}},
		{"room_tool.pen", ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_PLACE, "Place", "tool_place.png"}},
		{"room_tool.eraser", ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_ERASE, "Erase", "tool_erase.png"}},
		{"room_tool.edit", ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_EDIT, "Edit", "tool_edit.png"}},
		{"room_tool.set_spoint",
		 ToolboxItem<RoomTool>{"tool", RoomTool::TOOL_STARTPOINT, "Start Point", "tool_startpoint.png"}}};

	for (auto &[k, tool] : tools) {
		auto capturedTool = tool;
		toolbox->addTool(tool);
		hotkeyEntries.push_back(hks.registerHotkeyCallback(k, [this, capturedTool, toolbox]() {
			if (fileViewFocused) toolbox->selectTool(capturedTool);
		}));
	}

	auto brushToggle = tgui::CheckBox::create();
	bindTranslation<tgui::CheckBox>(brushToggle, "screen.project.roomview.enable_brush", &tgui::CheckBox::setText);
	brushToggle->onChange([this](bool toggled) { roomView->setBrush(toggled); });
	auto brushToggleSize = TOOLBOX_H - toolbox->getRenderer()->getPadding().getTop();
	brushToggle->setSize({brushToggleSize, brushToggleSize});
	toolbox->addWidget(brushToggle);
	hotkeyEntries.push_back(hks.registerHotkeyCallback("room_tool.toggle_bm", [this, brushToggle]() {
		if (fileViewFocused) brushToggle->setChecked(!brushToggle->isChecked());
	}));

	toolbox->onItemClicked([this](ToolboxItem<RoomTool> tool) { setRoomTool(tool); });

	widgetContainer.push_back(toolbox);
}

void RoomFileView::setRoomTool(ToolboxItem<RoomTool> tool) {
	tileSetView->setTool(tool.id);
	roomView->setTool(tool.id);
	layerVisitor.tool = tool.id;
	layerVisitor.group->removeAllWidgets();
	mj::visit(layerVisitor, static_cast<RoomLayer>(layerChoose->getSelectedItemIndex()));
}
RoomFileView::~RoomFileView() {
	HotkeyService &hks = Editor::instance->getHotkeyService();
	for (const auto &entry : hotkeyEntries) {
		hks.unregisterHotkeyCallback(entry);
	}
}

void RoomFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	if (variant == nullptr) return;

	auto ptr = dynamic_cast<Variant<Room> *>(variant);
	auto room = ptr->get();
	if (room == nullptr) return;

	roomView->setRoom(room);
	roomView->onAttributeChanged.connect([this] {
		this->dirty = true;
	});
	tileSetView->setTileSet(room->getTileMap()->getTileSet());
	widthField->value->setValue(room->getTileMap()->getMaxWorldSize().x);
	heightField->value->setValue(room->getTileMap()->getMaxWorldSize().y);
	tileSetField->value->setText(room->getTileMap()->getTileSetSource());
	musicFileField->value->setText(GetFileNameWithoutExt(room->getMusicSource().c_str()));

	addWidgets(layout);
	this->dirty = false;
}
