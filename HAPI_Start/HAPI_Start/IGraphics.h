#pragma once
#include <HAPI_lib.h>
#include "NGLOBALS.h"

using namespace HAPISPACE;




class IGraphics
{
public:
	//This or constructor?
	virtual void Init(int screenW, int screenH) = 0;

	virtual void ClearScreen(int r, int g, int b) = 0; //have alternate version of this function that only clears certain rectangle of the screen?

	virtual bool CreateSprite(std::string spriteName, const std::string& filename, bool shouldAnimate = false, std::string animSheetFilename = std::string()) = 0;
	virtual void DrawSprite(const std::string& spriteName, int x, int y, std::string animationName = "", int frameNumber = 0, SpriteEffect sprEffect = SpriteEffect::eNone) = 0;
	virtual void DrawSprite(const std::string& spriteName, int x, int y, bool repeating, eScrollType scrollType = eScrollType::Vertical, eScrollDirection scrollDir = eScrollDirection::Forward, int frame = 0) {};
	//virtual void DrawSprite(const std::string& spriteName, int x, int y, int frameNumber = 0, int rotation = 0);

	//get screen width
	//get screen height

	virtual int GetSpriteWidth(const std::string& spriteName) const = 0;
	virtual int GetSpriteHeight(const std::string& spriteName) const = 0;

	//Get sprite function?
};

