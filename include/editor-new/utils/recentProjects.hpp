#ifndef _EDITOR_UTILS_RECENTPROJECTS_H
#define _EDITOR_UTILS_RECENTPROJECTS_H

#include <deque>
#include <filesystem>
#include <string>

namespace editor {
const std::string RECENT_FILE = ".rpgpp_recent_project";

class RecentProjects {
private:
	int limit = 10;
	std::deque<std::string> recentProjects;
	std::filesystem::path path;
	void save();

public:
	RecentProjects();
	void enqueue(const std::string &projectPath);
	const std::deque<std::string> &getRecentProjects() const;
};
}  // namespace editor

#endif
