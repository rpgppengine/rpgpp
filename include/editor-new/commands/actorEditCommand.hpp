#ifndef _EDITOR_COMMANDS_ACTOREDITCOMMAND_H
#define _EDITOR_COMMANDS_ACTOREDITCOMMAND_H

#include "actor.hpp"
#include "basicTypes.hpp"
#include "utils/command.hpp"

namespace editor {
enum ActorCommandType { ACTOR_CMD_NEWFRAME, ACTOR_CMD_REMOVEFRAME, ACTOR_CMD_EDITFRAME };

class ActorEditCommand : public Command {
public:
	ActorCommandType commandType;
	Actor *actor;
	std::string animationId;
	int frameIndex = -1;

	IVector oldFrame;
	IVector newFrame;

	ActorEditCommand();

	void execute() override;
	void undo() override;
};
}  // namespace editor

#endif
