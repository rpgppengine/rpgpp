#ifndef _BASICTYPES_H
#define _BASICTYPES_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "cereal/cereal.hpp"
#include "raylib.h"

struct IVector {
	int x = 0;
	int y = 0;

	bool operator==(const IVector &other) const { return x == other.x && y == other.y; }

	bool operator<(const IVector &other) const { return x < other.x || (x == other.x && y < other.y); }

	IVector() = default;
	IVector(int x, int y) {
		this->x = x;
		this->y = y;
	}
	IVector(const Vector2 &from) {
		x = static_cast<int>(from.x);
		y = static_cast<int>(from.y);
	}

	Vector2 toFloat() const { return {static_cast<float>(x), static_cast<float>(y)}; }

	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(x), CEREAL_NVP(y));
	}
};

struct IRect {
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(x), CEREAL_NVP(y), CEREAL_NVP(width), CEREAL_NVP(height));
	}
};

struct UIElementRef {
	std::string title = "";
	uint8_t entityId = 255;

	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(title), CEREAL_NVP(entityId));
	}
};

struct FontRef {
	std::string path = "";
	Font font;
	int fontSize = 13;
	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(path), CEREAL_NVP(fontSize));
	}
};

struct ImageRef {
	std::string path = "";
	Texture texture;
	int scale = 1;
	void scaleImage(int newScale) {
		this->scale = newScale;
		auto loadedImage = LoadImage(TextFormat("images/%s", path.c_str()));
		ImageResizeNN(&loadedImage, loadedImage.width * scale, loadedImage.height * scale);
		texture = LoadTextureFromImage(loadedImage);
		UnloadImage(loadedImage);
	}

	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(path), CEREAL_NVP(scale));
	}
};

const std::string CALLBACK_TRIGGER = "callback_trigger";
const std::string CALLBACK_FOCUSED = "callback_focused";
const std::string CALLBACK_UNFOCUSED = "callback_unfocused";

#define RPGPP_CALLBACK_MAX (3)

struct CallbacksArray {
	std::unordered_map<std::string, std::string> funcNames = {
		{CALLBACK_TRIGGER, ""}, {CALLBACK_UNFOCUSED, ""}, {CALLBACK_FOCUSED, ""}};

	template <class Archive>
	void serialize(Archive &a) {
		a(cereal::make_nvp(CALLBACK_TRIGGER, funcNames[CALLBACK_TRIGGER]),
		  cereal::make_nvp(CALLBACK_FOCUSED, funcNames[CALLBACK_UNFOCUSED]),
		  cereal::make_nvp(CALLBACK_UNFOCUSED, funcNames[CALLBACK_UNFOCUSED]));
	}
};

struct InputC {
	UIElementRef upButton;
	UIElementRef downButton;
	UIElementRef leftButton;
	UIElementRef rightButton;

	CallbacksArray funcNames;

	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(upButton), CEREAL_NVP(downButton), CEREAL_NVP(leftButton), CEREAL_NVP(rightButton),
		  CEREAL_NVP(funcNames));
	}
};

enum TextAlignment {
	TEXT_ALIGN_LEFT = 0,
	TEXT_ALIGN_TOP = 0,
	TEXT_ALIGN_CENTRE = 1,
	TEXT_ALIGN_MIDDLE = 1,
	TEXT_ALIGN_RIGHT = 2,
	TEXT_ALIGN_BOTTOM = 2
};

struct VerticalAlignment {
	TextAlignment val = TEXT_ALIGN_TOP;

	template <class Archive>
	void serialize(Archive &a) {
		a(val);
	}
};

struct HorizontalAlignment {
	TextAlignment val = TEXT_ALIGN_LEFT;

	template <class Archive>
	void serialize(Archive &a) {
		a(val);
	}
};

enum ElementPropertyType {
	UI_PROP_INT,
	UI_PROP_BOOL,
	UI_PROP_STRING,
	UI_PROP_RECT,
	UI_PROP_COLOR,
	UI_PROP_FONT,
	UI_PROP_IMAGE,
	UI_PROP_REF,
	UI_PROP_NPATCHINFO,
	UI_PROP_INPUT
};

typedef std::vector<std::string> StringVector;

typedef std::variant<int, bool, std::string, Rectangle, Color, FontRef, ImageRef, VerticalAlignment,
					 HorizontalAlignment, UIElementRef, NPatchInfo, InputC, StringVector>
	ElementProperty;

struct Event {
	KeyboardKey key;
	bool hold = false;
};

template <class Archive>
void serialize(Archive &a, Rectangle &b) {
	a(cereal::make_nvp("x", b.x), cereal::make_nvp("y", b.y), cereal::make_nvp("width", b.width),
	  cereal::make_nvp("height", b.height));
}

template <class Archive>
void serialize(Archive &a, Vector2 &b) {
	a(cereal::make_nvp("x", b.x), cereal::make_nvp("y", b.y));
}

template <class Archive>
void serialize(Archive &a, Color &b) {
	a(cereal::make_nvp("a", b.a), cereal::make_nvp("r", b.r), cereal::make_nvp("g", b.g), cereal::make_nvp("b", b.b));
}

template <class Archive>
void serialize(Archive &a, NPatchInfo &b) {
	a(cereal::make_nvp("top", b.top), cereal::make_nvp("bottom", b.bottom), cereal::make_nvp("left", b.left),
	  cereal::make_nvp("right", b.right), cereal::make_nvp("layout", b.layout));
}

#endif
