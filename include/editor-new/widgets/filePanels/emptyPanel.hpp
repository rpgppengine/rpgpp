#ifndef _EDITOR_WIDGETS_FILEPANELS_EMPTYPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_EMPTYPANEL_H

#include <memory>

#include "edui/container.hpp"
#include "edui/label.hpp"
#include "widgets/filePanels/filePanel.hpp"
namespace editor {
class EmptyPanel : public FilePanel {
public:
	EmptyPanel();
	void init() override;

private:
	std::shared_ptr<edui::Label> label;
};
}  // namespace editor

#endif
