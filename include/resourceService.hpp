#ifndef _RPGPP_EDITOR_RESOURCESERVICE_H
#define _RPGPP_EDITOR_RESOURCESERVICE_H

#include <raylib.h>

#include <map>
#include <string>

class ResourceService {
private:
	std::map<std::string, Texture2D> textures;
	std::map<std::string, Font> fonts;

public:
	ResourceService();
	~ResourceService();
	void init();
	void addTextureFromFile(const std::string &filePath);
	void addTexture(const std::string &id, Texture2D texture);
	Texture2D getTexture(const std::string &id);
	bool textureExists(const std::string &id);
	void addFont(const std::string &id, Font font);
	void addFontFromFile(const std::string &filePath, int fontSize);
	Font getFont(const std::string &id);
	bool fontExists(const std::string &id);
	void unload() const;
};

#endif
