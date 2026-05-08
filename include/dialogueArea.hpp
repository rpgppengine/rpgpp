#ifndef _RPGPP_DIALOGUEAREA_H
#define _RPGPP_DIALOGUEAREA_H

#include "dialogueBalloon.hpp"
#include "raylib.h"
#include "uiElement.hpp"

class DialogueArea : public UIElement {
private:
	void putChar(Vector2 charMeasure, const char *c, DialogueLine &textLine, DialogueTextSection &textSection);
	void chooseSection(int i);

	int frameCounter = 0;

	Vector2 textPos;
	int charIndex = 0;
	int lineIndex = 0;
	int sectionIndex = 0;

	bool appliedTag = false;
	bool finishedTyping = false;
	bool firstCharTyped = false;
	float maxLineHeight = 0.0f;
	bool delay = false;
	float delayDuration = 0.0f;
	float padding = 0.0f;

	Font font;
	std::string fontName;

	bool dialogueFinished = false;

public:
	std::string text;
	Color textColor;
	std::string defaultFont;

	DialogueBin dialogue;
	DialogueLine *line = nullptr;
	DialogueTextSection *section = nullptr;

	DialogueArea();
	DialogueArea(Rectangle rect);

	void fromJson(const nlohmann::json &json) override;
	nlohmann::json dumpJson() override;
	void fromBin(UIElementBin &bin) override;
	UIElementBin dumpBin() override;
	std::map<std::string, xxx::any_ptr> getProps() override;

	void setText(const std::string &text);
	void setTextColor(const Color &color);
	void setDialogue(const DialogueBin &dialogue);
	bool isDialogueFinished();
	bool isFinishedTyping();

	void advanceToNextLine();

	void update() override;
	void draw() override;
};

#endif