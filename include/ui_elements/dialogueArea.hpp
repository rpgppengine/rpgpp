#ifndef _RPGPP_UI_ELEMENTS_DIALOGUEAREA_H
#define _RPGPP_UI_ELEMENTS_DIALOGUEAREA_H

#include "ui_elements/uiElement.hpp"

struct DialogueArea : public UIElement {
	DialogueArea();
	void init();
	void update();
	void draw(Rectangle rect);
	//
	std::string text = "";

	DialogueBin dialogue;
	DialogueLine *line = nullptr;
	DialogueTextSection *section = nullptr;

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

	FontRef font;

	bool dialogueFinished = false;

	void setDialogue(const DialogueBin &dialogue);
	void putChar(Vector2 charMeasure, const char *c, Rectangle rect);
	void chooseSection(int i, Rectangle rect);
	void advanceToNextLine();
};

#endif
