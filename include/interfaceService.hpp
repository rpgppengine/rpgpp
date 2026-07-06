#ifndef _RPGPP_INTERFACESERVICE_H
#define _RPGPP_INTERFACESERVICE_H

#include <raylib.h>

#include <map>
#include <memory>
#include <string>

#include "dialogueBalloon.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "ui_elements/elementFactory.hpp"

/** The InterfaceService acts for the User Interface (UI). */
class InterfaceService {
private:
	/** The loaded font that will be used for the User Interface. */
	Font font;
	bool fpsVisible;
	bool notifyLock = false;
	/** Helper for instantiating element types. */
	static ElementFactory factory;
	/** Available UI Views. */
	std::map<std::string, std::unique_ptr<InterfaceView>> views;
	/** Current active view. */
	std::string currentViewName;

public:
	/** Empty constructor. */
	InterfaceService();
	~InterfaceService();
	/** Initialize this service with a GameBin. */
	void initBin(GameData &bin);
	/** Get the font used for the game's UI. */
	Font getFont() const;
	/** Open a dialogue with a certain title. */
	void showDialogue(const std::string &id, bool runScript = true);
	/** Open the dialogue with a Dialogue structure */
	void showDialogue(const DialogueBin &dialogue, bool runScript = true);
	/** Get the current active view. */
	InterfaceView *getCurrentView();
	/** Set this view as the current one and show it. */
	void showInterface(const std::string &title, bool runScript = true);
	/** Hide the current view (no current view). */
	void hideInterface(bool runScript = true);
	void setNotifyLock();
	bool getNotifyLock();
	/** Get a reference to the factory helper. */
	static ElementFactory& getFactory();
	/** Update routine. */
	void update();
	/** Draw routine. */
	void draw();
	/** Unload routine. */
	void unload() const;
};

#endif
