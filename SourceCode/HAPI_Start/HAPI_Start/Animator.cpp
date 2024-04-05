#include "Animator.h"


void Animator::Animate(float dt)
{
	timer_frame += dt;
	if (timer_frame > milliseconds) //theoretically using >= instead of > should be a more expensive comparision
	{
		if (frame + 1 > maxFrames)
		{
			if (shouldLoop)
			{
				frame = 0;
				onCompleteSignal = true;
			}
			else
			{

				if (!completeSignalBeenTriggered)
				{
					onCompleteSignal = true;
					completeSignalBeenTriggered = true;
				}
				else
				{
					onCompleteSignal = false;
				}
			}
		}
		else
		{
			onCompleteSignal = false;
			completeSignalBeenTriggered = false;
			frame++;
		}

		timer_frame = 0;
	}
}

void Animator::SetAnimation(IGraphics& viz, std::string animationName, bool doNotResetFrameTime, bool ignoreIfIsCurrentAnimation)
{
	//ASSERT VIZ PTR NOT NULL
	assert(&viz != nullptr && "WARNING: GRAPHICS POINTER WAS NULL WHEN TRYING TO SET ANIMATION");
	//DON'T FORGET TO RE-IMPLEMENT THIS CHECK

	//cba to add the setanimation function as a pure virtual so just using a downcast that will
	//never fail bc there's only one visualisation class

	if (ignoreIfIsCurrentAnimation)
	{
		if (currentAnimationName == animationName) return;
	}

	if (!doNotResetFrameTime) frame = 0;


	timer_frame = 0;
	maxFrames = static_cast<Visualisation*>(&viz)->GetMaxFramesOfAnimation(graphicName, animationName) - 1;
	//unsafe but there is no check to see if an animation exists before setting the anim name

	if (doNotResetFrameTime)
	{
		if (frame > maxFrames) frame = maxFrames;
	}


	currentAnimationName = animationName;
	onCompleteSignal = false;
}
