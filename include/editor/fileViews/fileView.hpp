#ifndef _RPGPP_FILEVIEW_H
#define _RPGPP_FILEVIEW_H

#include <memory>
#include <stack>
#include <vector>

#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "actions/action.hpp"
#include "variant.hpp"
class FileView {
protected:
	VariantWrapper *variant;
	std::vector<tgui::Widget::Ptr> widgetContainer;

	std::stack<std::unique_ptr<Action>> past;
	std::stack<std::unique_ptr<Action>> future;

	bool dirty = false;

public:
	bool fileViewFocused = false;

	FileView();
	virtual ~FileView();

	virtual void init(tgui::Group::Ptr layout, VariantWrapper *variant) {}
	virtual void mouseMove(int x, int y) {}

	void addWidgets(tgui::Group::Ptr layout);
	void pushAction(std::unique_ptr<Action> action);
	void undoAction();
	void redoAction();

	void setDirty(bool value = true);
	bool getDirty();
};

#endif
