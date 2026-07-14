#include "fileViews/interfaceViewFileView.hpp"

#include <memory>
#include <vector>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/TabContainer.hpp"
#include "TGUI/Widgets/TreeView.hpp"
#include "bindTranslation.hpp"
#include "childWindows/elementInitWindow.hpp"
#include "childWindows/elementRenameWindow.hpp"
#include "editor.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
#include "views/interfaceViewView.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/fileField.hpp"

InterfaceViewFileView::InterfaceViewFileView() {
	editListFieldWindow = std::make_unique<EditListFieldWindow<std::string>>();

	auto createButton = tgui::Button::create("");
	bindTranslation<tgui::Button>(createButton, "screen.project.viewsview.create", &tgui::Button::setText);
	createButton->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});
	createButton->setSize({RIGHT_PANEL_W, 32});
	createButton->onClick([this] {
		auto *ptr = Editor::instance->getGui().getChildWindowSubService()->getWindow("create_ui_element");
		ElementInitWindow *window = static_cast<ElementInitWindow *>(ptr);
		window->view = view->ptr;
		window->tree = treeView.get();
		window->init();
		window->open();
	});
	widgetContainer.push_back(createButton);

	treeView = tgui::TreeView::create();
	treeView->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 32});
	treeView->setSize({RIGHT_PANEL_W, "50% - 32"});

	widgetContainer.push_back(treeView);

	auto toolPanel = tgui::Panel::create();
	toolPanel->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), 32});

	auto toolLayout = tgui::GrowHorizontalLayout::create();
	toolPanel->add(toolLayout);

	snapGridField = IntField::create();
	snapGridField->label->setText("Snap Grid");
	snapGridField->setSize({200, "100%"});
	snapGridField->value->setMinimum(2);
	snapGridField->value->setMaximum(16);
	snapGridField->value->setValue(4);

	toolLayout->add(snapGridField);

	widgetContainer.push_back(toolPanel);

	view = InterfaceViewView::create();
	view->setPosition({0, 32});
	view->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100% - 32"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(view));

	auto tabsContainer = tgui::TabContainer::create();
	tabsContainer->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), "50%"});
	tabsContainer->setSize({RIGHT_PANEL_W, "50%"});

	auto filePropertiesPanel = tabsContainer->addTab("UI View");
	auto elementPropertiesPanel = tabsContainer->addTab("Properties");

	widgetContainer.push_back(tabsContainer);

	// view properties
	scriptFileField = FileField::create();
	scriptFileField->setSize({"100%", 24});
	scriptFileField->label->setText("Script");
	scriptFileField->pathFilters = {{"Script", {"*.lua"}}};
	scriptFileField->callback = [this](const tgui::String &path) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(this->variant);
		const auto interface = ptr->get();

		std::string fileName = GetFileName(path.toStdString().c_str());
		interface->setScriptFile(fileName);
		this->dirty = true;
	};

	filePropertiesPanel->add(scriptFileField);

	// element properties
	propertiesBox = PropertiesBox::create();
	propertiesBox->setSize("100%", "100%");

	elementPropertiesPanel->add(propertiesBox);

	std::weak_ptr<tgui::TreeView> weakTree = treeView;
	std::weak_ptr<PropertiesBox> weakProps = propertiesBox;

	std::weak_ptr<InterfaceViewView> weakView = view;
	treeView->onItemSelect([weakView, this, weakProps](const tgui::String &item) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(this->variant);
		const auto interface = ptr->get();

		ElementIndex element = interface->findByName(item.toStdString());

		if (element >= MAX_ELEMENTS) return;

		if (auto ptr = weakView.lock()) {
			ptr->selectElement(item.toStdString());

			if (auto sharedProps = weakProps.lock()) {
				sharedProps->clear();

				visitProps(item.toStdString());
			}
		}
	});

	snapGridField->value->onValueChange([weakView] (int newValue) {
		if (auto sharedView = weakView.lock()) {
			sharedView->gridSnapSize = newValue;
		}
	});

	view->onActiveEntityChanged([weakTree, weakView, this, weakProps](ElementIndex entity) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(this->variant);
		const auto interface = ptr->get();

		if (auto ptr = weakTree.lock()) {
			auto sharedView = weakView.lock();
			if (entity >= MAX_ELEMENTS) {
				ptr->deselectItem();
				if (auto sharedProps = weakProps.lock()) {
					sharedProps->clear();
				}
			} else {
				ptr->selectItem({interface->getEntityName(entity)});
				if (!sharedView) return;

				if (auto sharedProps = weakProps.lock()) {
					sharedProps->clear();

					visitProps(interface->getEntityName(entity));
				}
			}
		}
	});

	treeView->onRightClick([this](const tgui::String &item) {
		selectedElement = item.toStdString();
		elementContextMenu->setPosition(GetMousePosition().x, GetMousePosition().y);
		elementContextMenu->openMenu();
	});

	elementContextMenu = tgui::ContextMenu::create();
	elementContextMenu->addMenuItem("Rename");
	elementContextMenu->addMenuItem("Move Up");
	elementContextMenu->addMenuItem("Move Down");
	elementContextMenu->addMenuItem("Delete");
	elementContextMenu->onMenuItemClick([this, weakTree](const std::vector<tgui::String> &hierarchy) {
		if (hierarchy.empty()) return;

		auto item = hierarchy[0];

		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
		const auto interface = ptr->get();

		auto screen = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());

		if (item == "Rename") {
			ElementIndex index = interface->findByName(selectedElement);

			auto windowPtr = Editor::instance->getGui().getChildWindowSubService()->getWindow("rename_ui_element");
			ElementRenameWindow* renameWindow = static_cast<ElementRenameWindow*>(windowPtr);

			renameWindow->view = interface;
			renameWindow->tree = treeView.get();
			renameWindow->elementIndex = index;

			renameWindow->init();
			renameWindow->open();
		}

		if (item == "Move Up") {
			ElementIndex index = interface->findByName(selectedElement);

			auto action = std::make_unique<Action>();
			action->onAction = [this, interface, index] {
				interface->swapElements(index, index - 1);

				populateTree();
			};
			action->onUndo = [this, interface, index] {
				interface->swapElements(index, index - 1);

				populateTree();
			};

			screen->getCurrentFile().getView().pushAction(std::move(action));
		}

		if (item == "Move Down") {
			ElementIndex index = interface->findByName(selectedElement);

			auto action = std::make_unique<Action>();
			action->onAction = [this, interface, index] {
				interface->swapElements(index, index + 1);

				populateTree();
			};
			action->onUndo = [this, interface, index] {
				interface->swapElements(index, index + 1);

				populateTree();
			};

			screen->getCurrentFile().getView().pushAction(std::move(action));
		}

		if (item == "Delete") {
			UIElement* element = interface->getElement(selectedElement);
			auto elementProps = element->props;
			auto elementType = element->typeName;

			auto action = std::make_unique<Action>();
			action->onAction = [this, interface] {
				interface->removeElement(selectedElement);
				treeView->removeItem({selectedElement});
			};
			action->onUndo = [this, interface, elementType, elementProps] {
				ElementIndex newIndex = interface->addElement(selectedElement, elementType);
				interface->getElement(newIndex)->props = elementProps;
				treeView->addItem({selectedElement});
			};

			screen->getCurrentFile().getView().pushAction(std::move(action));
		}
	});
	Editor::instance->getGui().gui->add(elementContextMenu);

	view->propBox = propertiesBox.get();
	view->visitor = &visitor;

	widgetContainer.push_back(view);
}

void InterfaceViewFileView::visitProps(const std::string &title) {
	const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
	const auto interface = ptr->get();

	visitor.box = propertiesBox.get();
	visitor.view = interface;
	visitor.editListFieldWindow = editListFieldWindow.get();

	auto element = interface->getElement(title);
	visitor.element = element;
	visitor.currentItemId = interface->findByName(title);
	if (element == nullptr) return;
	for (auto &[title, variant] : element->props) {
		visitor.key = title;
		std::visit(visitor, variant);
	}
}

void InterfaceViewFileView::populateTree() {
	const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
	const auto interface = ptr->get();

	treeView->removeAllItems();

	ElementIndex i = 0;
	for (auto &entity : interface->getElements()) {
		if (interface->getElement(i) != nullptr) {
			treeView->addItem({interface->getEntityName(i)});
			i++;
		}
	}
}

void InterfaceViewFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;
	this->visitor.screen = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<InterfaceView> *>(variant);
		const auto interface = ptr->get();

		visitor.view = interface;

		view->setInterfaceView(interface);

		treeView->removeAllItems();

		ElementIndex i = 0;
		for (auto &entity : interface->getElements()) {
			if (interface->getElement(i) != nullptr) {
				treeView->addItem({interface->getEntityName(i)});
				i++;
			}
		}

		scriptFileField->setValue(interface->getScriptFile());

		addWidgets(layout);
	}
}
