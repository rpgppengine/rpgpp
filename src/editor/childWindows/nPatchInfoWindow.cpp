#include "childWindows/nPatchInfoWindow.hpp"

#include "childWindows/popupWindow.hpp"
#include "editor.hpp"
#include "views/nPatchView.hpp"
#include "views/worldView.hpp"

NPatchInfoWindow::NPatchInfoWindow() : PopupWindow("NPatchInfoWindow") {
	currentWindow->setSize(480, 340);

	view = NPatchView::create();
	view->setSize({"100%", "100%"});

	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(view));
	currentWindow->add(view);
}
