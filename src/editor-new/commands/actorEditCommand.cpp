#include "commands/actorEditCommand.hpp"

#include "animation.hpp"

using namespace editor;

ActorEditCommand::ActorEditCommand() {}

void ActorEditCommand::execute() {
	Animation &anim = actor->getAnimationRef(animationId);
	oldFrame = anim.getFrame(frameIndex);

	switch (commandType) {
		case ACTOR_CMD_NEWFRAME:
			anim.insertFrame(frameIndex + 1, {0, 0});
			actor->setCurrentFrame(frameIndex + 1);
			break;
		case ACTOR_CMD_REMOVEFRAME:
			anim.removeFrame(frameIndex);
			actor->setCurrentFrame(0);
			break;
		case ACTOR_CMD_EDITFRAME:
			anim.setFrame(frameIndex, newFrame);
			break;
	}
}

void ActorEditCommand::undo() {
	Animation &anim = actor->getAnimationRef(animationId);

	switch (commandType) {
		case ACTOR_CMD_NEWFRAME:
			anim.removeFrame(frameIndex + 1);
			break;
		case ACTOR_CMD_REMOVEFRAME:
			anim.removeFrame(frameIndex);
			break;
		case ACTOR_CMD_EDITFRAME:
			anim.setFrame(frameIndex, oldFrame);
			break;
	}
}
