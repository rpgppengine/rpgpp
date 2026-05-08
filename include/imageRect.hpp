#ifndef _RPGPP_IMAGERECT_H
#define _RPGPP_IMAGERECT_H

#include <raylib.h>

#include <string>

#include "uiElement.hpp"

class ImageRect : public UIElement {
private:
	Texture2D texture;
	void loadTexture();

public:
	std::string source;
	int scale;

	ImageRect();
	ImageRect(Rectangle rect);

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setSource(const std::string &source);
	void setTexture(Texture2D texture);

	void update() override;
	void draw() override;
};

#endif
