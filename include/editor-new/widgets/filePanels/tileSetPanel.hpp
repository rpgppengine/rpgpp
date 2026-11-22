#ifndef _EDITOR_WIDGETS_FILEPANELS_TILESETPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_TILESETPANEL_H

#include <memory>

#include "widgets/filePanels/filePanel.hpp"
#include "widgets/propertyList.hpp"
#include "widgets/worldView/tileSetCanvas.hpp"
namespace editor {
enum TileSetPropId { TILESET_WIDTH = 1, TILESET_HEIGHT, TILESET_IMG };

class TileSetPanel : public FilePanel {
public:
	TileSetPanel();
	void init() override;
	virtual void onOpen() override;
	virtual void onClose() override;

private:
	const int PROPERTIES_WIDTH = 350;
	std::shared_ptr<PropertyList> properties;
	std::shared_ptr<TileSetCanvas> canvas;
};
}  // namespace editor

#endif
