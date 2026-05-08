#ifndef _RPGPP_COLORRECT_H
#define _RPGPP_COLORRECT_H

#include <raylib.h>

#include "gamedata.hpp"
#include "uiElement.hpp"

class ColorRect : public UIElement {
public:
	Color color;
	Color borderColor;
	int borderWidth;

	ColorRect();
	explicit ColorRect(Rectangle rect);

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setColor(Color newColor);
	void update() override;
	void draw() override;
};

#endif
