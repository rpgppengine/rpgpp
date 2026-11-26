#include "utils/variantPropertyFactory.hpp"

#include <string>

#include "basicTypes.hpp"
#include "raylib.h"
#include "widgets/values/boolValue.hpp"
#include "widgets/values/colorValue.hpp"
#include "widgets/values/fontRefValue.hpp"
#include "widgets/values/horizontalAlignValue.hpp"
#include "widgets/values/imageRefValue.hpp"
#include "widgets/values/intValue.hpp"
#include "widgets/values/rectangleValue.hpp"
#include "widgets/values/stringValue.hpp"
#include "widgets/values/tileSetRefValue.hpp"
#include "widgets/values/vector2value.hpp"
#include "widgets/values/verticalAlignValue.hpp"

using namespace editor;

VariantPropertyFactory::VariantPropertyFactory() {
	addEntry<int, IntValue>();
	addEntry<bool, BoolValue>();
	addEntry<Color, ColorValue>();
	addEntry<std::string, StringValue>();
	addEntry<Vector2, Vector2Value>();
	addEntry<HorizontalAlignment, HorizontalAlignValue>();
	addEntry<VerticalAlignment, VerticalAlignValue>();
	addEntry<FontRef, FontRefValue>();
	addEntry<ImageRef, ImageRefValue>();
	addEntry<TileSetRef, TileSetRefValue>();
	addEntry<Rectangle, RectangleValue>();
}
