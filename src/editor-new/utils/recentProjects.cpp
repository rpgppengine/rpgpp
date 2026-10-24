#include "utils/recentProjects.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "raylib.h"

using namespace editor;

RecentProjects::RecentProjects() {
	path = GetWorkingDirectory();
	path /= RECENT_FILE;

	if (!std::filesystem::exists(path)) {
		std::ofstream file(path);
		file.close();
	}

	std::ifstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open recent project file" << std::endl;
		return;
	}

	std::string s;
	while (std::getline(file, s)) {
		if (!std::filesystem::exists(s)) {
			continue;
		}
		recentProjects.push_back(s);
	}
	file.close();
	save();
}

void RecentProjects::save() {
	std::ofstream file(path);
	if (!file.is_open()) {
		std::cerr << "Failed to open recent project file for saving" << std::endl;
		return;
	}

	for (auto i = recentProjects.begin(); i != recentProjects.end(); ++i) {
		file << *i << std::endl;
	}

	file.close();
}

void RecentProjects::enqueue(const std::string &projectPath) {
	for (auto i = recentProjects.begin(); i != recentProjects.end(); ++i) {
		if (*i == projectPath) {
			recentProjects.erase(i);
			break;
		}
	}
	recentProjects.push_front(projectPath);
	if (recentProjects.size() > limit) {
		recentProjects.pop_back();
	}
	save();
}

const std::deque<std::string> &RecentProjects::getRecentProjects() const { return recentProjects; }
