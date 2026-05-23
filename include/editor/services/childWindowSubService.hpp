#ifndef RPGPP_CHILDWINDOWSERVICE_H
#define RPGPP_CHILDWINDOWSERVICE_H
#include <map>
#include <string>

#include "childWindows/popupWindow.hpp"

class ChildWindowSubService {
private:
	std::unordered_map<std::string, std::unique_ptr<PopupWindow>> childWindows = {};

public:
	ChildWindowSubService();
	void createWindows();
	void openWindow(const std::string &windowName);
	void resetAndOpen(const std::string windowName);
	PopupWindow *getWindow(const std::string &windowName);
};

#endif /* RPGPP_CHILDWINDOWSERVICE_H */
