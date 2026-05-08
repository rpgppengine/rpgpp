#ifndef _RPGPP_NINEPATCHIMAGERECT_H
#define _RPGPP_NINEPATCHIMAGERECT_H

#include <raylib.h>

#include <string>

#include "uiElement.hpp"

class NinePatchImageRect : public UIElement {
private:
	Texture2D texture;
	void loadTexture();

public:
	std::string source;
	int scale;
	NPatchInfo npatchInfo;

	NinePatchImageRect();
	NinePatchImageRect(Rectangle rect);

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setSource(const std::string &source);
	void setTexture(Texture2D texture);
	void setScale(int scale);

	void update() override;
	void draw() override;
};

#endif