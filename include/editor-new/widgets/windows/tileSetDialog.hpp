#ifndef _EDITOR_WINDOWS_TILESETDIALOG_H
#define _EDITOR_WINDOWS_TILESETDIALOG_H

#include <memory>

#include "basicTypes.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/signal.hpp"
#include "tileset.hpp"
#include "widgets/worldView/tileSetCanvas.hpp"
namespace editor {
struct TileSetDialog : public edui::ConfirmDialog {
	edui::SignalT<IVector> onConfirmed;

	TileSetDialog();
	void init(TileSet &tileSet);
	void setSelectedTile(IVector &tile);

private:
	std::shared_ptr<TileSetCanvas> canvas;
};
}  // namespace editor

#endif
