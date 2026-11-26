#ifndef _EDITOR_WIDGETS_FILEPANELS_ACTORPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_ACTORPANEL_H

#include "widgets/filePanels/propertyFilePanel.hpp"
namespace editor {
enum ActorPropertyId { ACTOR_TILESET = 1, ACTOR_COLLISION_RECT };

class ActorPanel : public PropertyFilePanel {
public:
	ActorPanel();
	void init() override;

private:
	const int PROPERTIES_WIDTH = 350;
};
}  // namespace editor

#endif
