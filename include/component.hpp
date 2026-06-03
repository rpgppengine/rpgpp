#ifndef _RPGPP_COMPONENT_H
#define _RPGPP_COMPONENT_H

#include <bitset>
#include <cstdint>
#include <functional>

#include "gamedata.hpp"
#include "raylib.h"

typedef std::uint8_t ComponentType;

const ComponentType MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct InputComponent {
	UIElementRef upButton;
	UIElementRef downButton;
	UIElementRef leftButton;
	UIElementRef rightButton;

	std::map<CallbackType, std::function<void()>> callbacks = {};
};

struct LabelComponent {
	std::string text = "";
	Color textColor = BLACK;
	TextAlignment horizontalAlignment = TEXT_ALIGN_TOP;
	TextAlignment verticalAlignment = TEXT_ALIGN_LEFT;

	FontRef font;
};

struct TextAreaComponent {
	std::string text = "";
	Color textColor = BLACK;

	FontRef font;
};

struct ColorRectComponent {
	Color color = RAYWHITE;
};

struct ImageRectComponent {
	ImageRef image;
};

struct NinePatchImageRectComponent {
	NPatchInfo npatchInfo = {{0, 0, 0, 0}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
	ImageRef image;
};

struct DialogueComponent {
	std::string text = "";
	Color textColor = BLACK;
	FontRef defaultFont;

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

	void putChar(Vector2 charMeasure, const char *c, Rectangle rect);
	void chooseSection(int i, Rectangle rect);
	void advanceToNextLine();
};

struct ButtonComponent {
	Color normalTextColor = BLACK;
	Color focusedTextColor = GRAY;

	Color shownTextColor = BLACK;
};

#endif