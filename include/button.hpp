#ifndef _RPGPP_BUTTON_H
#define _RPGPP_BUTTON_H

#include "colorRect.hpp"
#include "gamedata.hpp"
#include "label.hpp"
#include "raylib.h"
#include "uiElement.hpp"

class Button : public UIElement {
public:
	Label label;
	ColorRect colorRect;
	Color normalTextColor;
	Color focusedTextColor;
	Color shownTextColor;

	Button();
	Button(Rectangle rect);

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setRect(const Rectangle &rect) override;
	Rectangle getRect() override;
	void setText(const std::string &text);
	std::string getText();
	void setTextSize(int size);
	void setBackgroundColor(Color color);
	void setNormalTextColor(Color color);
	void setFocusedTextColor(Color color);

	void update() override;
	void draw() override;
};

#endif