#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <raylib.h>

#include <string>
#include <vector>

enum DialoguePaddingMode { PADDING_PX, PADDING_PERCENT };

struct DialogueTextSection {
	std::string key;
	std::string text;
	Color textColor = WHITE;
	int textSize = 13;
	std::string font = "LanaPixel";
	float delay = 0.0f;
	std::string sound = "Text 1";
	float padding = 0.0f;
	DialoguePaddingMode paddingMode = PADDING_PX;
	bool newline = false;
};

struct DialogueOption {
	std::string title;
	std::string nextDialogue;
};

struct DialogueLine {
	std::string characterName;
	std::string text;
	bool hasPortrait;
	std::string imageId;
	std::vector<DialogueTextSection> sections;
	bool hasOptions = false;
	std::vector<DialogueOption> options;
};

struct DialogueBin {
	std::string title;
	std::vector<DialogueLine> lines;
};

#endif
