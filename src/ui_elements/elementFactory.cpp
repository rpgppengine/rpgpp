#include "ui_elements/elementFactory.hpp"
#include "ui_elements/button.hpp"
#include "ui_elements/colorRect.hpp"
#include "ui_elements/dialogueArea.hpp"
#include "ui_elements/imageRect.hpp"
#include "ui_elements/label.hpp"
#include "ui_elements/ninePatchImageRect.hpp"
#include "valueSlider.hpp"

ElementFactory::ElementFactory() {
	map["ColorRect"] = [] { return std::make_unique<ColorRect>(); };
	map["Label"] = [] { return std::make_unique<Label>(); };
	map["ImageRect"] = [] { return std::make_unique<ImageRect>(); };
	map["NinePatchImageRect"] = [] { return std::make_unique<NinePatchImageRect>(); };
	map["Button"] = [] { return std::make_unique<Button>(); };
	map["DialogueArea"] = [] { return std::make_unique<DialogueArea>(); };
	map["ValueSlider"] = [] { return std::make_unique<ValueSlider>(); };

	for (auto& pair : map) {
		elementNames.emplace_front(pair.first);
	}
}

std::unique_ptr<UIElement> ElementFactory::constructElement(const std::string& title) {
	return std::move(map[title]());
}
