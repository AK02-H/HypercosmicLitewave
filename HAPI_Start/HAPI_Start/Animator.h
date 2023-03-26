#pragma once
#include <cassert>
#include "Visualisation.h"

class Visualisation;

class Animator
{
private:

	int fps{ 0 };
	float milliseconds{ 0 };
	float timer_frame{ 0 };

	std::string currentAnimationName = "";
	bool completeSignalBeenTriggered = false;

public:
	
	std::string graphicName;
	int frame{ 0 };
	int maxFrames{ 0 };

	bool shouldLoop{ true };
	bool onCompleteSignal{ false };

	Animator() {};

	//don't forget to set the animation after the construction
	Animator(std::string gfxName, int framerate = 12)
	{
		graphicName = gfxName;
		fps = framerate;
		milliseconds = (float)1 / fps;
	}

	std::string GetCurrentAnimationName() const { return currentAnimationName; }

	void Animate(float dt);
	
	void SetFPS(int newFps)
	{
		fps = newFps;
		milliseconds = (float)1 / fps;
	}

	void SetAnimationFrame(int frameNumber) //difference between this and setting frame directly is that this resets the timer
	{
		timer_frame = 0;
		frame = frameNumber;
		onCompleteSignal = false;
	}

	void SetAnimation(IGraphics& viz, std::string animationName, bool doNotResetFrameTime = false, bool ignoreIfIsCurrentAnimation = false);

	//to prevent really specific errors, avoid using
	void EmergencySetAnimName(std::string name) { currentAnimationName = name; }
};

