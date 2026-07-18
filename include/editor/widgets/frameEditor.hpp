#ifndef RPGPP_FRAMEEDITOR_H
#define RPGPP_FRAMEEDITOR_H

#include <memory>
#include <vector>

#include "TGUI/Signal.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "components/frameButton.hpp"
#include "views/actorView.hpp"

class ActorFileView;
class FrameEditor : public tgui::ScrollablePanel {
private:
	ActorFileView *fileView;
	ActorView::Ptr actorView;

	tgui::ComboBox::Ptr directionChooser;
	tgui::ToggleButton::Ptr playPauseButton;

	tgui::GrowHorizontalLayout::Ptr frameLayout;
	std::vector<FrameButton::Ptr> frameButtons;

public:
	FrameEditor(ActorFileView *fileView, const char *typeName = "FileChooser", bool initRenderer = true);

	typedef std::shared_ptr<FrameEditor> Ptr;
	static FrameEditor::Ptr create(ActorFileView *fileView);
	tgui::Signal onAttributeChanged = {"OnAttributeChanged"};

	void init();

	void onFrameChange(int currentFrame);

	void changeFrameState(int index);
	void updateFrameButtons(bool reImport = false);

	void addFrameButton(int index);

protected:
	tgui::Widget::Ptr clone() const override;
};

#endif
