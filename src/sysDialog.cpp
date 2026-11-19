#include "sysDialogs.hpp"
#include "tinyfiledialogs.h"

void NotifyPopup(const std::string &title, const std::string &message, DialogIconType icon) {
	std::string iconStr = "error";

	switch (icon) {
		case DialogIconType::INFO:
			iconStr = "info";
			break;
		case DialogIconType::WARNING:
			iconStr = "warning";
			break;
		case DialogIconType::ERROR:
			iconStr = "error";
			break;
	}

	tinyfd_notifyPopup(title.c_str(), message.c_str(), iconStr.c_str());
}

void MessageBox(const std::string &title, const std::string &message, MessageType dialogType, MessageIconType iconType,
				MessageDefaultButton defaultButton) {
	std::string dialogTypeStr = "ok";
	switch (dialogType) {
		case MessageType::OK:
			dialogTypeStr = "ok";
			break;
		case MessageType::OKCANCEL:
			dialogTypeStr = "okcancel";
			break;
		case MessageType::YESNO:
			dialogTypeStr = "yesno";
			break;
		case MessageType::YESNOCANCEL:
			dialogTypeStr = "yesnocancel";
			break;
	}

	std::string iconStr = "error";
	switch (iconType) {
		case MessageIconType::INFO:
			iconStr = "info";
			break;
		case MessageIconType::WARNING:
			iconStr = "warning";
			break;
		case MessageIconType::ERROR:
			iconStr = "error";
			break;
		case MessageIconType::QUESTION:
			iconStr = "question";
			break;
	}

	tinyfd_messageBox(title.c_str(), message.c_str(), dialogTypeStr.c_str(), iconStr.c_str(),
					  static_cast<int>(defaultButton));
}

bool InputBox(const std::string &title, const std::string &message, const std::string &defaultInput) {
	auto chr = tinyfd_inputBox(title.c_str(), message.c_str(), defaultInput.c_str());
	return chr;
}
