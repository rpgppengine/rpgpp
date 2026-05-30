#include "services/childWindowSubService.hpp"

#include <memory>

#include "childWindows/aboutWindow.hpp"
#include "childWindows/addDelayDialogueWindow.hpp"
#include "childWindows/addDialogueOptionWindow.hpp"
#include "childWindows/colorSelectWindow.hpp"
#include "childWindows/editDialogueOptionWindow.hpp"
#include "childWindows/editPropWindow.hpp"
#include "childWindows/nPatchInfoWindow.hpp"
#include "childWindows/newPropWindow.hpp"
#include "childWindows/popupWindow.hpp"
#include "childWindows/projectSettingsWindow.hpp"
#include "childWindows/settingsWindow.hpp"
#include "childWindows/uiElementRefWindow.hpp"

ChildWindowSubService::ChildWindowSubService() { this->createWindows(); }

void ChildWindowSubService::createWindows() {
	if (!this->childWindows.empty()) this->childWindows.clear();

	this->childWindows.try_emplace("about", std::make_unique<AboutWindow>());

	this->childWindows.try_emplace("options", std::make_unique<SettingsWindow>());

	this->childWindows.try_emplace("project_settings", std::make_unique<ProjectSettingsWindow>());

	this->childWindows.try_emplace("new_prop", std::make_unique<NewPropWindow>());

	this->childWindows.try_emplace("edit_prop", std::make_unique<EditPropWindow>());

	this->childWindows.try_emplace("select_a_color", std::make_unique<ColorSelectWindow>());

	this->childWindows.try_emplace("add_a_delay", std::make_unique<AddDelayDialogueWindow>());

	this->childWindows.try_emplace("add_dialogue_option", std::make_unique<AddDialogueOptionWindow>());

	this->childWindows.try_emplace("edit_dialogue_option", std::make_unique<EditDialogueOptionWindow>());

	this->childWindows.try_emplace("element_ref", std::make_unique<UIElementRefWindow>());

	this->childWindows.try_emplace("edit_npatch", std::make_unique<NPatchInfoWindow>());
}

void ChildWindowSubService::openWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1) this->childWindows[windowName]->open();
}

void ChildWindowSubService::resetAndOpen(const std::string windowName) {
	this->createWindows();
	this->openWindow(windowName);
}

PopupWindow *ChildWindowSubService::getWindow(const std::string &windowName) {
	if (this->childWindows.count(windowName) == 1) {
		return this->childWindows[windowName].get();
	} else {
		return nullptr;
	}
}
