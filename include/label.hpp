#ifndef _RPGPP_LABEL_H
#define _RPGPP_LABEL_H

#include "gamedata.hpp"
#include "raylib.h"
#include "uiElement.hpp"

class Label : public UIElement {
public:
	Font font;

	std::string text;
	Color textColor;
	TextAlignment horizontalAlignment;
	TextAlignment verticalAlignment;
	std::string fontName;
	int fontSize;

	Label();
	Label(Rectangle rect);

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setText(const std::string &text);

	void update() override;
	void draw() override;
};

#endif