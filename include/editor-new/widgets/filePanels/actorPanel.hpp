#ifndef _EDITOR_WIDGETS_FILEPANELS_ACTORPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_ACTORPANEL_H

#include <memory>

#include "widgets/actorAnimationsPanel.hpp"
#include "widgets/filePanels/propertyFilePanel.hpp"
#include "widgets/worldView/actorCanvas.hpp"

namespace editor {
enum ActorPropertyId { ACTOR_TILESET = 1, ACTOR_COLLISION_RECT };

class ActorPanel : public PropertyFilePanel {
public:
	ActorPanel();
	void init() override;
	void refresh() override;

private:
	const int PROPERTIES_WIDTH = 350;
	const int ANIMATIONS_HEIGHT = 220;

	std::shared_ptr<ActorCanvas> canvas;
	std::shared_ptr<ActorAnimationsPanel> animationsPanel;
};
}  // namespace editor

#endif
