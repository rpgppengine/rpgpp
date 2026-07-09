#ifndef _RPGPP_ROOMVIEW_H
#define _RPGPP_ROOMVIEW_H

#include <memory>

#include "TGUI/Signal.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "fileViews/fileView.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "room.hpp"
#include "roomLayerViewVisitor.hpp"
#include "tileSetView.hpp"
#include "tilemap.hpp"
#include "views/worldView.hpp"

class RoomViewModesHandler;

class RoomView : public WorldView {
	friend class RoomViewModesHandler;

private:
	RoomLayer layer;
	IVector selectedTile{-1, -1};
	bool brushMode = false;
	IVector getTileAtMouse();
	bool isTileValid(IVector tile);
	Rectangle getSourceRect(TileMap *tileMap, int x, int y);
	Rectangle getDestRect(TileMap *tileMap, int x, int y);
	void handleMode(int x, int y);
	void handlePlaceMode(int x, int y);
	void handleEraseMode(int x, int y);
	void handleEditMode(int x, int y);
	void handleModePress(tgui::Vector2f pos);
	void handleEditPress(tgui::Vector2f pos);

public:
	typedef std::shared_ptr<RoomView> Ptr;

	Room *room{nullptr};
	TileSetView *tileSetView{nullptr};
	FileView *fileView;
	tgui::ComboBox *interactableChoose;
	tgui::ComboBox *propChoose;
	tgui::Signal onAttributeChanged = {"OnAttributeChanged"};
	RoomLayerViewVisitor *layerVisitor;

	RoomView();

	void drawCanvas() override;
	void drawOverlay() override;
	static RoomView::Ptr create();
	static RoomView::Ptr create(Room *room);
	void setRoom(Room *room);
	Room *getRoom();

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	void keyPressed(const tgui::Event::KeyEvent &event) override;

	void setBrush(bool value);
	void setLayer(RoomLayer layer);
};

#endif
