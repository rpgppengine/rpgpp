#ifndef _RPGPP_INTERFACEVIEW_H
#define _RPGPP_INTERFACEVIEW_H

#include <raylib.h>

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "coordinator.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "saveable.hpp"
#include "uiElement.hpp"

class InterfaceView : public ISaveable {
private:
	Rectangle rect;
	std::multimap<int, std::unique_ptr<UIElement>, std::less<int>> elements;

	Coordinator ecs;

protected:
	UIElement *focused = nullptr;
	std::string focusedElementName = "";

public:
	InterfaceView();
	explicit InterfaceView(Rectangle rect);

	InterfaceView(const std::string &filePath);
	InterfaceView(InterfaceViewBin &bin);

	nlohmann::json dumpJson();

	virtual void onNotify(Event event);

	bool elementExists(const std::string &title);
	void addElement(const std::string &title, UIElement *element, int layer);
	void addElement(const std::string &title, std::unique_ptr<UIElement> element, int layer);
	void removeElement(const std::string &title);
	UIElement *getElement(const std::string &title);
	void renameElement(const std::string &title, const std::string &newTitle);
	void changeFocusedElement(const std::string &title);
	const std::multimap<int, std::unique_ptr<UIElement>, std::less<int>> &getElements();

	virtual void update();
	virtual void draw();

	const Coordinator &getCoordinator();
	const std::set<EntityID> &getEntities();
};

#endif
