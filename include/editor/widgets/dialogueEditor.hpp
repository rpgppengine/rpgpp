#ifndef _RPGPP_DIALOGUEEDITOR_HPP
#define _RPGPP_DIALOGUEEDITOR_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/TextArea.hpp"

struct XMLTagProperties {
	std::string propertyName;
	std::string propertyValue;
};

class DialogueEditor : public tgui::TextArea {
private:
	std::size_t formattingSelectionStart;
	std::size_t formattingSelectionEnd;
	tgui::String selectedText;

public:
	typedef std::shared_ptr<DialogueEditor> Ptr;
	DialogueEditor();

	static DialogueEditor::Ptr create();

	/**
	 * Add the specified XML tag to the user's selected text.
	 * @param tagName The name of the XML tag to add.
	 */
	void addXmlTag(std::string tagName);

	/**
	 * Add the specified XML tag with properties to the user's selected text.
	 * @param tagName The name of the XML tag to add.
	 * @param properties The properties to add to the XML tag.
	 */
	void addXmlTagWithProperties(std::string tagName, std::vector<XMLTagProperties> properties);
	/**
	 * This sets a text and resets the current selection.
	 * @param text Text to set.
	 */
	void setTextAndReset(std::string text);
	/**
	 * This fixes an interesting case with TGUI .w.
	 */
	void fixSelectionRange();
	bool isTextNonEditable(std::string &tagName);
};

#endif /* _RPGPP_DIALOGUEEDITOR_HPP */
