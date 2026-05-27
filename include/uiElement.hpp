#ifndef _RPGPP_UIELEMENT_H
#define _RPGPP_UIELEMENT_H

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "any_ptr/any_ptr.h"
#include "gamedata.hpp"
#include "jsonConversions.hpp"
#include "nlohmann/json.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "saveable.hpp"

struct Event {
	KeyboardKey key;
};

#define RPGPP_CALLBACK_TYPE_MAX (3)

class UIElement : public ISaveable {
protected:
	int layer = 0;
	bool focusable = false;
	std::string name = "";
	bool visible = true;

public:
	InterfaceElementType elementType;

	Rectangle rect;

	UIElementRef upButton;
	UIElementRef downButton;
	UIElementRef leftButton;
	UIElementRef rightButton;

	std::map<CallbackType, std::function<void()>> callbacks;

	UIElement();
	UIElement(InterfaceElementType elementType);
	virtual ~UIElement() = default;
	InterfaceElementType getType();
	virtual void onNotify(Event event);
	bool isFocusable();
	int getLayer();
	void setLayer(int layer);
	std::string getName();
	void setName(const std::string &name);

	bool isVisible();
	void setVisible(bool value);

	virtual Rectangle getRect();
	virtual void setRect(const Rectangle &rect);

	void invokeCallback(CallbackType type);
	void setCallback(CallbackType type, std::function<void()> callback);

	void setUpElement(const std::string &title);
	void setDownElement(const std::string &title);
	void setLeftElement(const std::string &title);
	void setRightElement(const std::string &title);

	virtual void update();
	virtual void draw();

	virtual nlohmann::json dumpJson();
	virtual void fromJson(const nlohmann::json &json);
	virtual UIElementBin dumpBin();
	virtual void fromBin(UIElementBin &bin);
	virtual std::map<std::string, xxx::any_ptr> getProps();

	nlohmann::json dumpNavigationJson();
	void fromNavigationJson(const nlohmann::json &json);
	UIElementBin dumpNavigationBin();
	void fromNavigationBin(UIElementBin &bin);
};

#endif
