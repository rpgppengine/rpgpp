#ifndef _EDITOR_COMMANDS_ACTOREDITCOMMAND_H
#define _EDITOR_COMMANDS_ACTOREDITCOMMAND_H

#include "actor.hpp"
#include "animation.hpp"
#include "basicTypes.hpp"
#include "utils/command.hpp"

namespace editor {
enum ActorCommandType {
	ACTOR_CMD_NEWFRAME,
	ACTOR_CMD_REMOVEFRAME,
	ACTOR_CMD_EDITFRAME,
	ACTOR_CMD_NEWANIM,
	ACTOR_CMD_REMOVEANIM
};

class ActorEditCommand : public Command {
public:
	ActorCommandType commandType;
	Actor *actor;
	Animation oldAnim;
	std::string animationId;
	int frameIndex = -1;

	IVector oldFrame;
	IVector newFrame;
	std::string newAnimId;

	ActorEditCommand();

	void execute() override;
	void undo() override;
};
}  // namespace editor

#endif
