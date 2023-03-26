#pragma once

#include "IGraphics.h"
#include "NGLOBALS.h"
#include <unordered_map>
//#include "Sprite.h"

class Sprite;

class Visualisation : public IGraphics
{
private:
	BYTE* m_screen{ nullptr };
	int m_screenWidth, m_screenHeight;
	std::unordered_map<std::string, Sprite*> m_spriteMap;

public:

	~Visualisation()
	{
		m_spriteMap.clear();
	}

	//This or constructor?
	void Init(int screenW, int screenH) override final;

	void ClearScreen(int r, int g, int b) override final; //have alternate version of this function that only clears certain rectangle of the screen?

	bool CreateSprite(std::string spriteName, const std::string& filename, bool shouldAnimate = false, std::string animSheetFilename = std::string()) override final;
	void DrawSprite(const std::string& spriteName, int x, int y, std::string animationName = "", int frameNumber = 0, SpriteEffect sprEffect = SpriteEffect::eNone) override final;
	void DrawSprite(const std::string& spriteName, int x, int y, bool repeating, eScrollType scrollType, eScrollDirection scrollDir, int frame = 0) override final;

	int GetSpriteWidth(const std::string& spriteName) const override final;
	int GetSpriteHeight(const std::string& spriteName) const override final;
	
	int GetMaxFramesOfAnimation(std::string& spriteName, std::string& animationName);
};

