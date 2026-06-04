#ifndef _RPGPP_JSONCONVERSIONS_H
#define _RPGPP_JSONCONVERSIONS_H

#include <nlohmann/json.hpp>

#include "component.hpp"
#include "gamedata.hpp"
#include "raylib.h"

class UIElement;

using json = nlohmann::json;

// property types
void to_json(json &j, const Rectangle &rect);
void from_json(const json &j, Rectangle &rect);

void to_json(json &j, const Color &color);
void from_json(const json &j, Color &color);

void to_json(json &j, const ImageRef &image);
void from_json(const json &j, ImageRef &image);

void to_json(json &j, const FontRef &font);
void from_json(const json &j, FontRef &font);

void to_json(json &j, const NPatchInfo &info);
void from_json(const json &j, NPatchInfo &info);

void to_json(json &j, const UIElementRef &info);
void from_json(const json &j, UIElementRef &info);

// components
void to_json(json &j, const InputComponent &input);
void from_json(const json &j, InputComponent &input);

void to_json(json &j, const VisibilityComponent &visibility);
void from_json(const json &j, VisibilityComponent &visibility);

void to_json(json &j, const LabelComponent &label);
void from_json(const json &j, LabelComponent &label);

void to_json(json &j, const TextAreaComponent &textArea);
void from_json(const json &j, TextAreaComponent &textArea);

void to_json(json &j, const ColorRectComponent &color);
void from_json(const json &j, ColorRectComponent &color);

void to_json(json &j, const ImageRectComponent &image);
void from_json(const json &j, ImageRectComponent &image);

void to_json(json &j, const NinePatchImageRectComponent &ninePatch);
void from_json(const json &j, NinePatchImageRectComponent &ninePatch);

void to_json(json &j, const DialogueComponent &dialogue);
void from_json(const json &j, DialogueComponent &dialogue);

void to_json(json &j, const ButtonComponent &button);
void from_json(const json &j, ButtonComponent &button);

#endif