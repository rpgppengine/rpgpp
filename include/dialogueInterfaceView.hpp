#ifndef _RPGPP_DIALOGUEINTERFACEVIEW_H
#define _RPGPP_DIALOGUEINTERFACEVIEW_H

#include <vector>

#include "dialogue.hpp"
#include "dialogueBalloon.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"

class DialogueInterfaceView : public InterfaceView {
public:
	DialogueBin dialogue;
	int optionsCount = 0;

	DialogueInterfaceView();

	DialogueInterfaceView(const std::string &filePath);
	DialogueInterfaceView(InterfaceViewBin &bin);

	void setDialogue(DialogueBin dialogue);

	void onNotify(Event event);

	void update();
	void draw();
};

#endif