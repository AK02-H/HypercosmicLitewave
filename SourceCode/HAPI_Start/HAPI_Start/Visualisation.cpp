#include "Visualisation.h"
#include "Sprite.h"

void Visualisation::Init(int screenW, int screenH)
{
	m_screen = HAPI.GetScreenPointer();
	m_screenWidth = screenW;
	m_screenHeight = screenH;
}

void Visualisation::ClearScreen(int r, int g, int b)
{
	assert(m_screen != nullptr);

	HAPI_TColour col(r, g, b);

	for (size_t i = 0; i < m_screenWidth * (m_screenHeight / 8); i++)
	{

		memcpy(m_screen + i * 4, &col, 4);

	}

	for (size_t i = 0; i < 3; i++)
	{
		int newSrc = (m_screenWidth * ((m_screenHeight / 8) * (pow(2, i)) * 4));
		memcpy(m_screen + newSrc, m_screen, newSrc);

	}
}

bool Visualisation::CreateSprite(std::string spriteName, const std::string& filename, bool shouldAnimate, std::string animSheetFilename)
{
	if (spriteName == "")
	{
		HAPI.UserMessage("Warning: cannot give sprite blank name", "Error");
		return false;
	}


	Sprite* newSprite = new Sprite();
	if (!newSprite->LoadTexture(filename))
	{
		std::cerr << "Texture not found" << std::endl;
		//Skip this if false and load default texture instead?

		delete newSprite;
		return false;

		//load default tex
	}

	if (shouldAnimate)
	{
		newSprite->doAnimate = true;
		newSprite->LoadAnimationSheetData(animSheetFilename); //no way to check if this animation loading fails currently
	};


	m_spriteMap[spriteName] = newSprite;

	return true;
	//return false;
}

void Visualisation::DrawSprite(const std::string& spriteName, int x, int y, std::string animationName, int frameNumber, SpriteEffect sprEffect)
{
	//Check if sprite exists, create it if it doesnt?


	if (m_spriteMap.find(spriteName) == m_spriteMap.end())
	{
		std::cerr << "SPRITE: " << spriteName << " - NOT FOUND" << std::endl;
		//get default sprite?
		return;
	}
	m_spriteMap[spriteName]->Draw(m_screen, x, y, true, animationName, frameNumber, sprEffect);

}

void Visualisation::DrawSprite(const std::string& spriteName, int x, int y, bool repeating, eScrollType scrollType, eScrollDirection scrollDir, int frame)
{

	if (m_spriteMap.find(spriteName) == m_spriteMap.end())
	{
		std::cerr << "SPRITE: " << spriteName << " - NOT FOUND" << std::endl;
		//get default sprite?
		//return;
		m_spriteMap[NULLTEX_STR]->DrawRepeating(m_screen, x, y, true, scrollType, scrollDir);
		return;
	}

	if (repeating)
	{
		m_spriteMap[spriteName]->DrawRepeating(m_screen, x, y, true, scrollType, scrollDir);
	}
	else {} //???
}


int Visualisation::GetSpriteWidth(const std::string& spriteName) const
{
	if (m_spriteMap.find(spriteName) == m_spriteMap.end())
	{
		return -1;
	}
	return m_spriteMap.at(spriteName)->GetWidth();
}

int Visualisation::GetSpriteHeight(const std::string& spriteName) const
{
	if (m_spriteMap.find(spriteName) == m_spriteMap.end())
	{
		return -1;
	}
	return m_spriteMap.at(spriteName)->GetHeight();
}

int Visualisation::GetMaxFramesOfAnimation(std::string& spriteName, std::string& animationName)
{
	if (m_spriteMap.find(spriteName) == m_spriteMap.end())
	{
		//sprite not found, send message
		HAPI.UserMessage("Error when getting sprite animation frame count, sprite " + spriteName + " not found in sprite.", "Warning");
		return 0;
	}
	else
	{
		if (m_spriteMap.find(spriteName)->second->GetAllAnimations()->find(animationName) == m_spriteMap.find(spriteName)->second->GetAllAnimations()->end())
		{
			//animation not found, send message
			HAPI.UserMessage("Error when getting sprite animation frame count, animation " + animationName + " not found in sprite.", "Warning");
			return 0;
		}

		return m_spriteMap.find(spriteName)->second->GetAnimation(animationName)->size();

	}
}
;