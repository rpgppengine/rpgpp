#ifndef _EDITOR_SERVICES_RESOURCES_H
#define _EDITOR_SERVICES_RESOURCES_H

#include <map>
#include <string>

#include "raylib.h"
#include "utils/recentProjects.hpp"
namespace editor {
class Resources {
public:
	Resources();

	void unload();

	void loadTexture(const std::string &title, const std::string &path);
	void loadFont(const std::string &title, const std::string &path);

	Texture getTexture(const std::string &title);
	Font getFont(const std::string &title);

	RecentProjects &getRecentProjects();

private:
	void loadResources();
	std::map<std::string, Texture> textures;
	std::map<std::string, Font> fonts;

	RecentProjects recentProjects;
};
}  // namespace editor

#endif
