#ifndef _RPGPP_PROJECTSETTINGSPANEL_GAME_H
#define _RPGPP_PROJECTSETTINGSPANEL_GAME_H

#include <memory>

#include "TGUI/Widgets/Label.hpp"
#include "childWindows/editListFieldWindow.hpp"
#include "childWindows/settingsPanel/base.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/listField.hpp"

class ProjectSettingsPanelGame : public SettingsPanelBase {
public:
	FileField::Ptr defaultLoading;
	FileField::Ptr playerActor;
	IntField::Ptr tileSize;
	BoolField::Ptr debugDraw;
	ListField<int>::Ptr exportImageScales;
	ListField<int>::Ptr exportFontSizes;

	std::unique_ptr<EditListFieldWindow<int>> editListFieldWindow;

	ProjectSettingsPanelGame(tgui::TabContainer::Ptr tabContainer);
	void setup(Project *project);
};

#endif
