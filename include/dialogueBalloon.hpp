#ifndef _RPGPP_DIALOGUEBALLOON_H
#define _RPGPP_DIALOGUEBALLOON_H

#include <raylib.h>

#include <string>
#include <vector>

#include "cereal/cereal.hpp"

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

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(key), CEREAL_NVP(text), CEREAL_NVP(textColor), CEREAL_NVP(textSize), CEREAL_NVP(font),
		  CEREAL_NVP(delay), CEREAL_NVP(sound), CEREAL_NVP(sound), CEREAL_NVP(padding), CEREAL_NVP(paddingMode),
		  CEREAL_NVP(newline));
	}
};

struct DialogueOption {
	std::string title;
	std::string nextDialogue;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(title), CEREAL_NVP(nextDialogue));
	}
};

struct DialogueLine {
	std::string characterName;
	std::string text;
	bool hasPortrait;
	std::string imageId;
	std::vector<DialogueTextSection> sections;
	bool hasOptions = false;
	std::vector<DialogueOption> options;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(characterName), CEREAL_NVP(text), CEREAL_NVP(hasPortrait), CEREAL_NVP(imageId),
		  CEREAL_NVP(sections), CEREAL_NVP(hasOptions), CEREAL_NVP(options));
	}
};

struct DialogueBin {
	std::string title;
	std::vector<DialogueLine> lines;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(title), CEREAL_NVP(lines));
	}
};

#endif
