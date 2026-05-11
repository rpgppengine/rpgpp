#ifndef _RPGPP_FILEVIEWS_UIFILEVIEW_H
#define _RPGPP_FILEVIEWS_UIFILEVIEW_H

#include <memory>

#include "TGUI/Widgets/TreeView.hpp"
#include "fileViews/fileView.hpp"
#include "nlohmann/json_fwd.hpp"
#include "views/interfaceViewView.hpp"
#include "widgets/propertiesBox.hpp"

class InterfaceViewFileView : public FileView {
private:
	static const int RIGHT_PANEL_W = 300;
	tgui::TreeView::Ptr treeView;
	PropertiesBox::Ptr propertiesBox;

	InterfaceViewView::Ptr view;

	std::unique_ptr<nlohmann::json> elementProps;

	sol::state luaState;

public:
	InterfaceViewFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
};

#endif