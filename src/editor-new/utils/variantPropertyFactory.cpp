#include "utils/variantPropertyFactory.hpp"

#include "edui/intValue.hpp"

using namespace editor;

VariantPropertyFactory::VariantPropertyFactory() { addEntry<int, edui::IntValue>(); }
