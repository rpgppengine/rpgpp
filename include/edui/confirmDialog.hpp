#ifndef _EDUI_CONFIRMDIALOG_H
#define _EDUI_CONFIRMDIALOG_H

#include "edui/messageBox.hpp"
#include "edui/signal.hpp"
namespace edui {
struct ConfirmDialogRender : public MessageBoxRender {};

struct ConfirmDialog : public MessageBox {
	ConfirmDialog();

	Signal onCancelPressed;
};
}

#endif
