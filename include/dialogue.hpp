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
	Dialogue();
	Dialogue(const std::string &filePath);
	DialogueLine addNewLine();
	// nlohmann::json dumpJson() override;
	DialogueBin &getData();

	template <typename Archive>
	void serialize(Archive &a) {
		dialogueBin.serialize(a);
	}
};

#endif
