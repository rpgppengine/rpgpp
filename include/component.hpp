#ifndef _RPGPP_COMPONENT_H
#define _RPGPP_COMPONENT_H

#include <bitset>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>

#include "dialogueBalloon.hpp"
#include "gamedata.hpp"
#include "raylib.h"

typedef std::uint8_t ComponentType;

const ComponentType MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct VisibilityComponent {
	bool isVisible = true;
};

struct InputComponent {
	UIElementRef upButton;
	UIElementRef downButton;
	UIElementRef leftButton;
	UIElementRef rightButton;

	CallbacksArray funcNames;
	std::map<CallbackType, std::function<void()>> callbacks = {{}, {}, {}};
};

struct LabelComponent {
	std::string text = "";
	Color textColor = BLACK;
	TextAlignment horizontalAlignment = TEXT_ALIGN_TOP;
	TextAlignment verticalAlignment = TEXT_ALIGN_LEFT;

	FontRef font;

	void loadFont(const std::string &path);
};

struct TextAreaComponent {
	std::string text = "";
	Color textColor = BLACK;

	FontRef font;

	void loadFont(const std::string &path);
};

struct ColorRectComponent {
	Color color = RAYWHITE;
};

struct ImageRectComponent {
	ImageRef image;
	void loadImage(const std::string &path);
	void scaleImage(int scale);
};

struct NinePatchImageRectComponent {
	NPatchInfo npatchInfo = {{0, 0, 0, 0}, 0, 0, 0, 0, NPATCH_NINE_PATCH};
	ImageRef image;
	void loadImage(const std::string &path);
	void scaleImage(int scale);
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

	void setDialogue(const DialogueBin &dialogue);
	void putChar(Vector2 charMeasure, const char *c, Rectangle rect);
	void chooseSection(int i, Rectangle rect);
	void advanceToNextLine();
};

struct ButtonComponent {
	Color normalTextColor = BLACK;
	Color focusedTextColor = GRAY;

	Color shownTextColor = BLACK;

	void setNormalTextColor(Color color);
};

#endif