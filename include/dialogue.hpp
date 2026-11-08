#ifndef _RPGPP_DIALOGUE
#define _RPGPP_DIALOGUE

#include <filesystem>
#include <nlohmann/json_fwd.hpp>

#include "dialogueBalloon.hpp"
#include "dialogueParser.hpp"
#include "resource.hpp"
#include "saveable.hpp"

class Dialogue : public Resource {
private:
	DialogueBin dialogueBin;

public:
	Dialogue() = default;
	Dialogue(const std::string &filePath);
	DialogueLine addNewLine();
	// nlohmann::json dumpJson() override;
	DialogueBin &getData();

	template <typename Archive>
	void serialize(Archive &a) {
		dialogueBin.serialize(a);
	}

	void loadFile(std::filesystem::path filePath) override {
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
	void saveFile(std::filesystem::path filePath) override {}
};

#endif
