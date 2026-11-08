#ifndef _EDITOR_SERVICES_RESOURCES_H
#define _EDITOR_SERVICES_RESOURCES_H

#include <map>
#include <string>

#include "raylib.h"
#include "utils/project.hpp"
#include "utils/recentProjects.hpp"
#include "utils/resourceTypeManager.hpp"
namespace editor {
class Resources {
public:
	Resources();
	void update();

	void unload();

	void loadTexture(const std::string &title, const std::string &path);
	void loadFont(const std::string &title, const std::string &path);

	Texture getTexture(const std::string &title);
	Font getFont(const std::string &title);

	Project &getProject();
	RecentProjects &getRecentProjects();
	ResourceTypeManager &getTypes();

	bool fileClipboardAvailable();
	void setFileClipboard(const std::string &filePath);
	std::string getFileClipboard();

	void onNewProject();
	void onOpenProject();

private:
	void loadResources();
	std::map<std::string, Texture> textures;
	std::map<std::string, Font> fonts;

	bool hasFileClipboard = false;
	std::string filePathClipboard = "";

	Project openedProject;
	RecentProjects recentProjects;
	ResourceTypeManager typeManager;
};
}  // namespace editor

#endif
