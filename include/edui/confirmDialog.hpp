#ifndef _EDUI_CONFIRMDIALOG_H
#define _EDUI_CONFIRMDIALOG_H

#include "edui/button.hpp"
#include "edui/messageBox.hpp"
#include "edui/signal.hpp"
namespace edui {
struct ConfirmDialogRender : public MessageBoxRender {};

/* A window widget which has an 'OK' and 'Cancel' button. */
struct ConfirmDialog : public MessageBox {
	const int CANCEL_BUTTON_ID = 2;

	ConfirmDialog();

	Signal onCancelPressed;

	void translate() override;

	/* Get a reference to the Cancel button. */
	edui::Button &getCancelButton();
};
}  // namespace edui

#endif
