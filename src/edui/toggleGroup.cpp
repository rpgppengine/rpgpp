#include "edui/toggleGroup.hpp"

#include <memory>

#include "edui/horizontalContainer.hpp"
#include "edui/toggleButton.hpp"

using namespace edui;

ToggleGroup::ToggleGroup() : HorizontalContainer() { render = std::make_unique<edui::ToggleGroupRender>(); }

void ToggleGroup::applyTheme(const mINI::INIStructure &struc) {
	HorizontalContainer::applyTheme(struc);
	for (auto &widget : widgets) {
		widget->as<ToggleButton>().resizeToFit();
	}
}

void ToggleGroup::addItem(const std::string &text, int iconId) {
	auto newItem = std::make_shared<ToggleButton>();
	newItem->iconId = iconId;
	newItem->setText(text);
	newItem->resizeToFit();
	newItem->eventLock = true;

	int size = widgets.size();
	newItem->onClicked.connect([this, size] { toggleItem(size); });

	add(newItem);
}

void ToggleGroup::toggleItem(int index) {
	if (auto sharedActive = activeItem.lock()) {
		if (sharedActive != nullptr) {
			sharedActive->as<ToggleButton>().setInactive();
			sharedActive->unfocused();
		}
	}

	widgets[index]->as<ToggleButton>().setActive();
	activeItem = widgets[index];
}
