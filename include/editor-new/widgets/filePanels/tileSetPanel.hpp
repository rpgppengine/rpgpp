#ifndef _EDITOR_WIDGETS_FILEPANELS_TILESETPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_TILESETPANEL_H

#include <memory>

#include "commands/propSet.hpp"
#include "tileset.hpp"
#include "widgets/filePanels/filePanel.hpp"
#include "widgets/filePanels/propertyFilePanel.hpp"
#include "widgets/propertyList.hpp"
#include "widgets/values/valuewidget.hpp"
#include "widgets/worldView/tileSetCanvas.hpp"
namespace editor {
enum TileSetPropId { TILESET_WIDTH = 1, TILESET_HEIGHT, TILESET_IMG };

class TileSetPanel : public PropertyFilePanel {
public:
	TileSetPanel();
	void init() override;
	void refresh() override;

private:
	const int PROPERTIES_WIDTH = 350;
	std::shared_ptr<TileSetCanvas> canvas;

	void initTexture();
};
}  // namespace editor

#endif
