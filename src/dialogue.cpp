#include "dialogue.hpp"

#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

#include "dialogueBalloon.hpp"
#include "dialogueParser.hpp"
#include "raylib.h"

Dialogue::Dialogue(const std::string &filePath) {
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

/*
nlohmann::json Dialogue::dumpJson() {
	std::vector<nlohmann::json> diagVec;
	for (DialogueLine line : dialogueBin.lines) {
		auto lineVec = nlohmann::json::object();
		lineVec.push_back({"characterName", line.characterName});
		lineVec.push_back({"text", line.text});
		lineVec.push_back({"hasPortrait", line.hasPortrait});
		lineVec.push_back({"imageId", line.imageId});
		lineVec.push_back({"hasOptions", line.hasOptions});

		auto optionsVec = std::vector<nlohmann::json>{};

		for (auto &option : line.options) {
			auto optionsObj = nlohmann::json::object();
			optionsObj["name"] = option.title;
			optionsObj["nextDialogue"] = option.nextDialogue;
			optionsVec.push_back(optionsObj);
		}
		lineVec.push_back({"options", optionsVec});

		diagVec.push_back(lineVec);
	}

	json result = {{"diag", diagVec}};

	return result;
}
*/

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
