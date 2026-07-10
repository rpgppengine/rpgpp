#include "fileViews/fileView.hpp"

#include <memory>
#include <utility>

#include "actions/action.hpp"

FileView::FileView() {}

FileView::~FileView() {}

void FileView::addWidgets(tgui::Group::Ptr layout) {
	for (auto widget : widgetContainer) {
		layout->add(widget);
	}
}

void FileView::pushAction(std::unique_ptr<Action> action) {
	if (action->executeOnAdd) {
		action->execute();
		dirty = true;
	}

	past.push(std::move(action));
}

void FileView::undoAction() {
	if (!past.empty()) {
		past.top()->undo();

		future.push(std::move(past.top()));
		past.pop();
	}
}

void FileView::redoAction() {
	if (!future.empty()) {
		future.top()->execute();

		past.push(std::move(future.top()));
		future.pop();
	}
}

void FileView::setDirty(bool value) {
	this->dirty = value;
}

bool FileView::getDirty() {
	return dirty;
}
