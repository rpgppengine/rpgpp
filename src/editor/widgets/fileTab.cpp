#include "widgets/fileTab.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

#include "TGUI/Color.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/Sprite.hpp"
#include "TGUI/Text.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "TGUI/Widgets/TabsBase.hpp"
#include "components/tooltip.hpp"
#include "editor.hpp"
#include "fileTabRenderer.hpp"
#include "raylib.h"

using namespace tgui;

FileTab::FileTab(const char *typeName, bool initRenderer) : tgui::Tabs(typeName, false) {
	m_distanceToSideCached = (std::round(Text::getLineHeight(m_fontCached, getGlobalTextSize()) * 2.f));
	if (initRenderer) {
		m_renderer = aurora::makeCopied<FileTabRenderer>();
		setRenderer(tgui::Theme::getDefault()->getRendererNoThrow("Tabs"));

		setTextSize(getGlobalTextSize());
		setTabHeight(std::round(Text::getLineHeight(m_fontCached, m_textSizeCached) * 1.25f) +
					 m_bordersCached.getTopPlusBottom());
	}
	tooltip = Tooltip::create("");
	// setToolTip(tooltip);
}

FileTab::Ptr FileTab::create() { return std::make_shared<FileTab>(); }

FileTab::Ptr FileTab::copy(FileTab::ConstPtr widget) {
	if (widget)
		return std::static_pointer_cast<FileTab>(widget->clone());
	else
		return nullptr;
}

tgui::TabsBase::Tab* FileTab::getTabId(const std::string fileName) {
	auto tab = std::find_if(m_tabs.begin(), m_tabs.end(), [fileName](const tgui::TabsBase::Tab tab) {
		return tab.id == fileName;
	});
	if (tab == m_tabs.end())
		return nullptr;
	return tab.base();
}

tgui::TabsBase::Tab* FileTab::getTabName(const std::string fileName) {
	auto tab = std::find_if(m_tabs.begin(), m_tabs.end(), [fileName](const tgui::TabsBase::Tab tab) {
		return tab.text.getString() == fileName;
	});
	if (tab == m_tabs.end())
		return nullptr;
	return tab.base();
}

void FileTab::closeTabFilename(const std::string fileName){
	auto tab = this->getTabName(fileName);

	auto indx = this->getIndexById(tab->id);
	// Remove the tab and move to the before tab of this current tab.
	this->remove(fileName);
	onTabClose.emit(this, tab->id);

	if (m_tabs.size() > 0) {
		select(std::max(0, indx - 1));
	}
}

void FileTab::closeAndOpenNextTab(std::size_t i) {
	int prevSelected = m_selectedTab;
	tgui::String prevId = m_tabs[i].id;
	remove(i);
	onTabClose.emit(this, prevId);
	if (prevSelected == i && m_tabs.size() > 0) {
		select(std::min(i, m_tabs.size() - 1));
	}
}

bool FileTab::leftMousePressed(Vector2f pos) {
	pos -= getPosition();

	float tabStart = 0.0f;
	float tabEnd = m_bordersCached.getLeft() / 2.f;
	for (std::size_t i = 0; i < m_tabs.size(); ++i) {
		if (!m_tabs[i].visible) continue;

		// Append the width of the tab
		tabEnd += (m_bordersCached.getLeft() / 2.f) + m_tabs[i].width + (m_bordersCached.getRight() / 2.0f);

		// If the mouse went down on this tab then select it
		if (pos.x >= tabStart && pos.x < tabEnd - CLOSE_BUTTON_SIZE - MARGIN_LR) {
			select(i);
			isHoldingMouse = true;
			startMousePos = pos;
			draggedTab = i;
			break;
		} else if (pos.x >= tabStart + (m_tabs[i].width - MARGIN_LR - CLOSE_BUTTON_SIZE) &&
				   pos.x < tabEnd - MARGIN_LR) {
			closeAndOpenNextTab(i);
			break;
		}

		tabStart += m_tabs[i].width;
	}

	return false;
}

void FileTab::manualMouseMoved(tgui::Vector2f pos) {
	Tabs::mouseMoved(pos);
	pos -= getPosition();
	deltaMousePos = pos - startMousePos + offsetMousePos;
	if (isHoldingMouse and
		(abs(deltaMousePos.x) >= BUFFER_BEFORE_TAB_MOVE.x or abs(deltaMousePos.y) >= BUFFER_BEFORE_TAB_MOVE.y)) {
		isDragging = true;
	}

	// if (m_hoveringTab > 0) tooltip->setText(m_tabs[m_hoveringTab].id);

	if (isDragging) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_ALL);
		cursorModified = true;
		if (draggedTab != m_hoveringTab and m_hoveringTab >= 0 and swappedTab == -1) {
			// adds the width between the currently hovering tab and the dragged
			// tab not counting the dragged tab
			for (int i = std::min(m_hoveringTab, draggedTab); i <= std::max(m_hoveringTab, draggedTab); i++) {
				if (i == draggedTab) continue;
				offsetMousePos.x -= m_tabs[i].width *						   // width of the hovered tab
									(deltaMousePos.x / abs(deltaMousePos.x));  // direction of the drag
			}
			deltaMousePos = pos - startMousePos + offsetMousePos;			   // value is updated immediately to
																			   // avoid flickering
			swappedTab = draggedTab;
			if (draggedTab < m_hoveringTab) {
				std::rotate(m_tabs.begin() + draggedTab, m_tabs.begin() + draggedTab + 1,
							m_tabs.begin() + m_hoveringTab + 1);
			} else if (draggedTab > m_hoveringTab) {
				std::rotate(m_tabs.begin() + m_hoveringTab, m_tabs.begin() + draggedTab,
							m_tabs.begin() + draggedTab + 1);
			}
			draggedTab = m_hoveringTab;
			m_selectedTab = draggedTab;
		} else if (swappedTab != m_hoveringTab) {
			swappedTab = -1;
		}
	} else if (cursorModified) {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		cursorModified = false;
	}
}

void FileTab::mouseMoved(tgui::Vector2f pos) {
	Widget::mouseMoved(pos);
	if (!useExternalMouseEvent) {
		manualMouseMoved(pos);
	}
}

void FileTab::manualLeftMouseReleased(tgui::Vector2f pos) {
	pos -= getPosition();
	isHoldingMouse = false;
	isDragging = false;
	offsetMousePos = {0, 0};
	swappedTab = -1;
}

void FileTab::leftMouseReleased(tgui::Vector2f pos) {
	Widget::leftMouseReleased(pos);
	if (!useExternalMouseEvent) {
		manualLeftMouseReleased(pos);
	}
}

void FileTab::closeCurrentTab() {
	if (m_selectedTab == -1) return;
	closeAndOpenNextTab(m_selectedTab);
}

bool FileTab::select(std::size_t index) {
	// Don't select a tab that is already selected
	if (m_selectedTab == static_cast<int>(index)) return true;

	// If the index is too high or if the tab is invisible or disabled then we
	// can't select it
	if ((index >= m_tabs.size()) || !m_enabled || !m_tabs[index].visible || !m_tabs[index].enabled) {
		deselect();
		return false;
	}

	// Select the tab
	m_selectedTab = static_cast<int>(index);
	m_tabs[index].text.setColor(m_selectedTextColorCached);
	updateTextColors();

	// Send the callback
	onTabSelect.emit(this, m_tabs[index].id);
	return true;
}

size_t FileTab::addFileTab(const std::string &path, const std::string &fileName) {
	size_t tabIdxToInsert = m_selectedTab;
	if (tabIdxToInsert == -1) {
		tabIdxToInsert = 0;
	}

	// Fix for the selection, as previously two fileTabs were being created.
	auto tabRef = this->getTabId(path);
	if (tabRef != nullptr || this->getTabName(fileName) != nullptr) {
		auto tabIndx = this->getIndexById(path);
		select(tabIndx);
		return -1;
	}

	tabIdxToInsert += 1;
	tabIdxToInsert = std::min(tabIdxToInsert, m_tabs.size());

	insert(tabIdxToInsert, fileName, true);
	m_tabs[tabIdxToInsert].id = path;
	return tabIdxToInsert;
}

void FileTab::renderTab(tgui::BackendRenderTarget &target, tgui::RenderStates &states, int i, bool roundedCorners,
						float borderWidth, float usableHeight, tgui::Sprite &close) const {
	if (!m_tabs[i].visible) return;

	RenderStates textStates = states;
	if (roundedCorners) textStates.transform.translate({0, m_bordersCached.getTop()});

	tgui::Color backgroundColor;
	if ((!m_enabled || !m_tabs[i].enabled) && m_backgroundColorDisabledCached.isSet())
		backgroundColor = m_backgroundColorDisabledCached;
	else if (m_selectedTab == static_cast<int>(i)) {
		if ((m_hoveringTab == static_cast<int>(i)) && m_selectedBackgroundColorHoverCached.isSet())
			backgroundColor = m_selectedBackgroundColorHoverCached;
		else
			backgroundColor = m_selectedBackgroundColorCached;
	} else if ((m_hoveringTab == static_cast<int>(i)) && m_backgroundColorHoverCached.isSet())
		backgroundColor = m_backgroundColorHoverCached;
	else
		backgroundColor = m_backgroundColorCached;

	// Draw the background of the tab
	const Sprite *spriteTab = nullptr;
	if ((!m_enabled || !m_tabs[i].enabled) && m_spriteDisabledTab.isSet())
		spriteTab = &m_spriteDisabledTab;
	else if ((m_selectedTab == static_cast<int>(i)) && m_spriteSelectedTab.isSet()) {
		if ((m_hoveringTab == static_cast<int>(i)) && m_spriteSelectedTabHover.isSet())
			spriteTab = &m_spriteSelectedTabHover;
		else
			spriteTab = &m_spriteSelectedTab;
	} else if ((m_hoveringTab == static_cast<int>(i)) && m_spriteTabHover.isSet())
		spriteTab = &m_spriteTabHover;
	else if (m_spriteTab.isSet())
		spriteTab = &m_spriteTab;

	if (roundedCorners) {
		states.transform.translate({-borderWidth, 0});
		target.drawRoundedRectangle(states, {m_tabs[i].width + (2 * borderWidth), getSize().y},
									tgui::Color::applyOpacity(backgroundColor, m_opacityCached),
									m_roundedBorderRadiusCached, m_bordersCached,
									tgui::Color::applyOpacity(m_borderColorCached, m_opacityCached));
		states.transform.translate({m_tabs[i].width + 2 * borderWidth, 0});
	} else {
		if (spriteTab) {
			Sprite spriteTabCopy = *spriteTab;
			spriteTabCopy.setSize({m_tabs[i].width, usableHeight});
			target.drawSprite(states, spriteTabCopy);

		} else	// No texture was loaded
			target.drawFilledRect(states, {m_tabs[i].width, usableHeight},
								  tgui::Color::applyOpacity(backgroundColor, m_opacityCached));

		// draw close button
		auto tabState = states;
		tabState.transform.translate(
			{m_tabs[i].width - CLOSE_BUTTON_SIZE - MARGIN_LR, (usableHeight - CLOSE_BUTTON_SIZE) / 2.f});
		close.setSize({CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE});
		target.drawSprite(tabState, close);

		// Draw the borders between the tabs
		if ((borderWidth != 0) && (i < m_tabs.size())) {
			target.drawBorders(states, Borders{borderWidth}, {m_tabs[i].width, usableHeight},
							   tgui::Color::applyOpacity(m_borderColorCached, m_opacityCached));
			// target.drawFilledRect(
			// 	states, {borderWidth, usableHeight},
			// 	tgui::Color::applyOpacity(m_borderColorCached,
			// 							  m_opacityCached));
			// states.transform.translate({borderWidth, 0});
		}
		states.transform.translate({m_tabs[i].width, 0});
	}

	// Highlight the borders of the selected and hovered tab when requested
	if (m_bordersCached != Borders{0}) {
		tgui::Color highlightColor;
		if ((m_hoveringTab == static_cast<int>(i)) && m_borderColorHoverCached.isSet())
			highlightColor = m_borderColorHoverCached;
		if (m_selectedTab == static_cast<int>(i)) {
			if ((m_hoveringTab == static_cast<int>(i)) && m_selectedBorderColorHoverCached.isSet())
				highlightColor = m_selectedBorderColorHoverCached;
			else if (m_selectedBorderColorCached.isSet())
				highlightColor = m_selectedBorderColorCached;
		}

		if (highlightColor.isSet()) {
			float leftBorderWidth = borderWidth;
			float rightBorderWidth = borderWidth;
			if (i == 0) leftBorderWidth = m_bordersCached.getLeft();
			if (i == m_tabs.size() - 1) rightBorderWidth = m_bordersCached.getRight();

			if ((m_selectedTab >= 0) && (m_hoveringTab >= 0) &&
				(m_borderColorHoverCached.isSet() &&
				 (m_selectedBorderColorCached.isSet() || m_selectedBorderColorHoverCached.isSet()))) {
				if ((m_selectedTab == static_cast<int>(i - 1)) || (m_hoveringTab == static_cast<int>(i - 1)))
					leftBorderWidth /= 2;
				else if ((m_selectedTab == static_cast<int>(i + 1)) || (m_hoveringTab == static_cast<int>(i + 1)))
					rightBorderWidth /= 2;
			}

			RenderStates highlightStates = states;
			if (i < m_tabs.size() - 1) highlightStates.transform.translate({-borderWidth, 0});
			highlightStates.transform.translate({-m_tabs[i].width - leftBorderWidth, -m_bordersCached.getTop()});
			target.drawBorders(
				highlightStates,
				{leftBorderWidth, m_bordersCached.getTop(), rightBorderWidth, m_bordersCached.getBottom()},
				{m_tabs[i].width + leftBorderWidth + rightBorderWidth, getSize().y}, highlightColor);
		}
	}

	// Apply clipping if required for the text in this tab
	const float usableWidth = m_tabs[i].width - (2 * m_distanceToSideCached);
	const bool clippingRequired = (m_tabs[i].text.getSize().x > usableWidth);
	if (clippingRequired)
		target.addClippingLayer(textStates, {{m_distanceToSideCached, 0}, {usableWidth, usableHeight}});

	// Draw the text
	textStates.transform.translate({m_distanceToSideCached + ((usableWidth - m_tabs[i].text.getSize().x) / 2.f),
									((usableHeight - m_tabs[i].text.getSize().y) / 2.f)});
	target.drawText(textStates, m_tabs[i].text);

	if (clippingRequired) target.removeClippingLayer();
}

void FileTab::draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const {
	auto closeImagePath = Editor::instance->getFs().getResourcePath("close.png");
	tgui::Texture closeTexture(closeImagePath);
	Sprite close(closeTexture);

	const float borderWidth = (m_bordersCached.getLeftPlusRight()) / 2.f;
	const bool roundedCorners = (m_roundedBorderRadiusCached > 0) && !m_spriteTab.isSet();
	if (!roundedCorners) {
		// // Draw the borders around the tabs
		// if (m_bordersCached != Borders{0}) {
		// 	target.drawBorders(states, m_bordersCached, getSize(),
		// 					   tgui::Color::applyOpacity(m_borderColorCached,
		// 												 m_opacityCached));
		// 	states.transform.translate(m_bordersCached.getOffset());
		// }
	} else
		states.transform.translate({borderWidth, 0});

	const float usableHeight = getSize().y;

	RenderStates draggingState;
	for (std::size_t i = 0; i < m_tabs.size(); ++i) {
		if (isDragging and i == draggedTab) {
			draggingState = states;
			states.transform.translate({m_tabs[i].width, 0});
			continue;
		}
		renderTab(target, states, i, roundedCorners, borderWidth, usableHeight, close);
	}
	if (isDragging) {
		draggingState.transform.translate({deltaMousePos.x, 0});
		renderTab(target, draggingState, draggedTab, roundedCorners, borderWidth, usableHeight, close);
		draggingState.transform.translate({-deltaMousePos.x, 0});
	}
}

tgui::Widget::Ptr FileTab::clone() const { return std::make_shared<FileTab>(*this); }

FileTabRenderer *FileTab::getSharedRenderer() {
	return aurora::downcast<FileTabRenderer *>(tgui::Widget::getSharedRenderer());
}

const FileTabRenderer *FileTab::getSharedRenderer() const {
	return aurora::downcast<const FileTabRenderer *>(tgui::Widget::getSharedRenderer());
}

FileTabRenderer *FileTab::getRenderer() { return aurora::downcast<FileTabRenderer *>(tgui::Widget::getRenderer()); }
