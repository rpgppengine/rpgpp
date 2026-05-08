#ifndef _RPGPP_TEXTAREA_H
#define _RPGPP_TEXTAREA_H

#include <raylib.h>

#include <string>

#include "uiElement.hpp"

class TextArea : public UIElement {
private:
	void putChar(int i, Vector2 *charPos, Vector2 *charMeasure) const;

public:
	std::string content;

	TextArea();
	TextArea(Rectangle rect);

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
