#ifndef _RPGPP_FILEVIEWS_UIFILEVIEW_H
#define _RPGPP_FILEVIEWS_UIFILEVIEW_H

#include <memory>

#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "fileViews/fileView.hpp"
#include "nlohmann/json_fwd.hpp"
#include "uiElementPropVisitor.hpp"
#include "views/interfaceViewView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/fileField.hpp"

class InterfaceViewFileView : public FileView {
private:
	static const int RIGHT_PANEL_W = 300;

	tgui::TreeView::Ptr treeView;
	FileField::Ptr scriptFileField;
	PropertiesBox::Ptr propertiesBox;
	InterfaceViewView::Ptr view;

	tgui::ContextMenu::Ptr elementContextMenu;
	std::string selectedElement;

	UIElementPropVisitor visitor;

public:
	InterfaceViewFileView();
	void init(tgui::Group::Ptr layout, VariantWrapper *variant) override;
	void visitProps(const std::string &title);
};

#endif
