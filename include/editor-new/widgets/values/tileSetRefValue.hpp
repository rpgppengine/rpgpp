#ifndef _EDITOR_WIDGETS_VALUES_TILESETREFVALUE_H
#define _EDITOR_WIDGETS_VALUES_TILESETREFVALUE_H

#include "basicTypes.hpp"
#include "edui/fileInput.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct TileSetRefValue : public ValueWidgetT<TileSetRef> {
	TileSetRefValue();
	TileSetRef getValue() override;
	void setValue(const TileSetRef &value) override;

private:
	std::shared_ptr<edui::FileInput> input;
};
}  // namespace editor

#endif
