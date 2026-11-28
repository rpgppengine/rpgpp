#include "commands/actorEditCommand.hpp"

#include "animation.hpp"

using namespace editor;

ActorEditCommand::ActorEditCommand() {}

void ActorEditCommand::execute() {
	Animation &anim = actor->getAnimationRef(animationId);
	oldFrame = anim.getFrame(frameIndex);
	oldAnim = anim;

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
		case ACTOR_CMD_NEWANIM:
			actor->addAnimation(newAnimId, {{0, 0}});
			actor->changeAnimation(newAnimId);
			break;
		case ACTOR_CMD_REMOVEANIM:
			actor->removeAnimation(animationId);
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
			anim.insertFrame(frameIndex, oldFrame);
			break;
		case ACTOR_CMD_EDITFRAME:
			anim.setFrame(frameIndex, oldFrame);
			break;
		case ACTOR_CMD_NEWANIM:
			actor->removeAnimation(newAnimId);
			break;
		case ACTOR_CMD_REMOVEANIM:
			actor->addAnimation(animationId, oldAnim.getFrames());
			break;
	}
}
