#pragma once

#include <HAPI_lib.h>
#include <cassert>#
#include <unordered_map>
#include "NGLOBALS.h"
#include "Rectangle.h"

using namespace HAPISPACE;


struct AnimationData
{
	std::string name;
	std::vector<RectStruct> sheetFrames;
};


class Sprite

{
private:

	//change hardcoding later, set in initialise function when created by visualisation
	int screenWidth{ SCR_WIDTH };
	int screenHeight{ SCR_HEIGHT };

	int m_width;
	int m_height;

	bool m_hasAlpha{ true };
	BYTE* m_texture{ nullptr };

	Rectangle texRect;
	Rectangle screenRect;

	int currentFrame{ 0 };	//get rid of this variable
	
	//individual animations are stored as a list of Rects that define which frames in the texture to use
	//all animations are stored in a map and referred to by name
	std::vector<Rectangle> m_allRects;
	std::unordered_map<std::string, std::vector<int>> m_allAnimations;
	std::vector<int>* currentAnimation;
	std::string currentAnimationName;

	//START ADDING CONST TO THESE FUNCTIONS AND PARAMETERS
	void BlitTransparent(BYTE* screen, BYTE* texture, int texW, int texH, int posX, int posY);
	void BlitFast(BYTE* screen, BYTE* texture, int texW, int texH, int posX, int posY);
	void BlitTransparent(BYTE* screen, BYTE* texture, const Rectangle& screenRect, const Rectangle& texRect, int posX, int posY, std::string animationName = "", int frame = 0, SpriteEffect sprEffect = SpriteEffect::eNone);	//move sprite effect arguement further up?
	void BlitRepeating(BYTE* screen, BYTE* texture, const Rectangle& screenRect, const Rectangle& texRect, int posX, int posY, eScrollType scrollType = eScrollType::Vertical, eScrollDirection scrollDir = eScrollDirection::Forward);
	void GeneralBlitCode(BYTE*& screen, BYTE*& texture, const Rectangle& clippedRect, int screenRectWidth, int posX, int posY, int& screenOffset, int& endOfLineOffset, int& endOfTexOffset, SpriteEffect sprEffect = SpriteEffect::eNone);
	void InitBlitOffsets(int& screenOffset, int& endOfLineOffset, int& endOfTexOffset, BYTE* &texture, int screenRectWidth, Rectangle& clippedRect, const Rectangle& texRect, int posX, int posY);

	Rectangle* GetAnimationRect(std::string animName, int frame)
	{
		//thisline was added to ignore the graphical assert that kept
		//coming up with the kamikaze enemy
		//if (frame >= currentAnimation->size()) frame = currentAnimation->size() - 1;	
		
		return &m_allRects[m_allAnimations[animName][frame]];
	}

public:

	bool doAnimate{ false };

	bool LoadTexture(const std::string& filename);
	bool LoadAnimationSheetData(const std::string& filename);

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	void Draw(BYTE* screen, int x, int y, bool transparent = true, std::string animationName = "", int frameNumber = 0, SpriteEffect sprEffect = SpriteEffect::eNone);
	void DrawRepeating(BYTE* screen, int x, int y, bool transparent = true, eScrollType scrollType = eScrollType::Vertical, eScrollDirection scrollDir = eScrollDirection::Forward, int frameNumber =  0);
	
	void AddNewAnimation(std::string& animationName, std::vector<int> animationFrames)
	{
		m_allAnimations.insert(std::pair < std::string, std::vector<int>>(animationName, animationFrames));
	}

	std::vector<int>* GetAnimation(std::string& animationName);
	

	std::unordered_map<std::string, std::vector<int>>* GetAllAnimations();


};

