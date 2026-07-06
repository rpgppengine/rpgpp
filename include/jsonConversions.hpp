#ifndef _RPGPP_JSONCONVERSIONS_H
#define _RPGPP_JSONCONVERSIONS_H

#include <nlohmann/json.hpp>

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

void to_json(json &j, const CallbacksArray &info);
void from_json(const json &j, CallbacksArray &info);

void to_json(json &j, const UIElementRef &info);
void from_json(const json &j, UIElementRef &info);

void to_json(json &j, const InputC &input);
void from_json(const json &j, InputC &input);

#endif
