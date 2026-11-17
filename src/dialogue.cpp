#include "dialogue.hpp"

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

#include "dialogueBalloon.hpp"
#include "dialogueParser.hpp"
#include "raylib.h"
#include "resource.hpp"

Dialogue::Dialogue() : Resource(RPGPP_RESOURCE_DIALOGUE) {}

Dialogue::Dialogue(const std::string &filePath) : Dialogue() {
	DialogueBin result;
	char *fileTxt = LoadFileText(filePath.c_str());

	try {
		json jsonObj = json::parse(fileTxt);
		std::vector<nlohmann::json> diagVector = jsonObj.at("diag");
		for (auto lineVec : diagVector) {
			DialogueLine line;
			line.characterName = lineVec.value("characterName", "Character");
			line.text = lineVec.value("text", "No text available!");
			line.hasPortrait = lineVec.value("hasPortrait", false);
			line.imageId = lineVec.value("imageId", "");

			line.sections = parseDialogueContent(line.text);

			line.hasOptions = lineVec.value("hasOptions", false);
			for (auto &jOption : lineVec.at("options")) {
				line.options.push_back({jOption.at("name"), jOption.at("nextDialogue")});
			}

			result.lines.push_back(line);
		}

		dialogueBin = result;
	} catch (json::exception e) {
		UnloadFileText(fileTxt);
		throw std::runtime_error("Failed to parse dialogue JSON");
	}
	UnloadFileText(fileTxt);
}

DialogueBin &Dialogue::getData() { return dialogueBin; }

DialogueLine Dialogue::addNewLine() {
	DialogueLine newLine;
	newLine.characterName = "Character";
	newLine.hasPortrait = false;
	newLine.imageId = "";
	newLine.text = "Enter text here!";

	dialogueBin.lines.push_back(newLine);

	return newLine;
}
