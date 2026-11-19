#ifndef _RPGPP_SYSDIALOGS_H
#define _RPGPP_SYSDIALOGS_H

#include <string>
enum class DialogIconType { INFO, WARNING, ERROR };
enum class MessageIconType { INFO, WARNING, ERROR, QUESTION };
enum class MessageType { OK, OKCANCEL, YESNO, YESNOCANCEL };
enum class MessageDefaultButton { CANCEL, OK, NO };

void NotifyPopup(const std::string &title, const std::string &message, DialogIconType icon);
void MessageBox(const std::string &title, const std::string &message, MessageType dialogType, MessageIconType iconType,
				MessageDefaultButton defaultButton);
bool InputBox(const std::string &title, const std::string &message, const std::string &defaultInput);

#endif
