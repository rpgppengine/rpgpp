#ifndef _RPGPP_COMPONENT_H
#define _RPGPP_COMPONENT_H

#include <bitset>
#include <cstdint>

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
};

struct LabelComponent {
	std::string text;
	Color textColor;
	TextAlignment horizontalAlignment;
	TextAlignment verticalAlignment;
	std::string fontName;
	int fontSize;

	Font font;
};

struct TextAreaComponent {
	std::string text;
	Color textColor;
	std::string fontName;
	int fontSize;

	Font font;
};

struct ColorRectComponent {
	Color color;
};

struct ImageRectComponent {
	std::string source;
	int scale;

	Texture2D texture;
};

struct NinePatchImageRectComponent {
	std::string source;
	int scale;
	NPatchInfo npatchInfo;

	Texture2D texture;
};

struct DialogueComponent {
	std::string text;
	Color textColor;
	std::string defaultFont;

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

	Font font;
	std::string fontName;

	bool dialogueFinished = false;

	void putChar(Vector2 charMeasure, const char *c, Rectangle rect);
	void chooseSection(int i, Rectangle rect);
	void advanceToNextLine();
};

#endif