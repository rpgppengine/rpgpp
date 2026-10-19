#ifndef _EDUI_CONFIRMDIALOG_H
#define _EDUI_CONFIRMDIALOG_H

#include "edui/button.hpp"
#include "edui/messageBox.hpp"
#include "edui/signal.hpp"
namespace edui {
struct ConfirmDialogRender : public MessageBoxRender {};

struct ConfirmDialog : public MessageBox {
	const int CANCEL_BUTTON_ID = 2;

	ConfirmDialog();

	Signal onCancelPressed;

	void translate() override;

	edui::Button &getCancelButton();
};
}  // namespace edui

#endif
