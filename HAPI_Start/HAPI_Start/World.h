#pragma once
#include <vector>
#include <HAPI_lib.h>
#include <cassert>
#include "IGraphics.h"
#include "Visualisation.h"
#include "InputHandler.h"
#include "Collider.h"
#include "Rectangle.h"

class Entity;

enum class EGraphicsSystem
{
	eHAPI,
	eOpenGL
};

enum class eGameReturnQ
{
	eRestart,
	eMenu, //realistically, no special code if it's this
	eQuitGame
};

enum class eMenuType
{
	eMain,
	e1P2Pselect,
	eGameSetup,
	eHighScores,
	eOptions
};

enum class eDifficulty
{
	eEasy,
	eNormal,
	eHard
};

enum class eGameSpeed
{
	e25,
	e50,
	e100,
	e150,
	e200,
};

class World
{
private:


	

	//Should I move these back to be member variables so they show up in the UML diagram? (m_ should be rid of if they stay here)
	std::vector<Entity*> m_entities;
	std::vector<InputHandler*> m_allInputHandlers;

	IGraphics* visual;
	IGraphics* visual_menu;

	bool m_isLocalMultiplayerGame = false;
	
	eDifficulty gameDifficulty = eDifficulty::eNormal;
	eGameSpeed gameSpeed = eGameSpeed::e100;

	//options
	int m_mxVolume = 70;
	int m_sfxVolume = 90;
	int controlSettingsID = 0;

	bool stereoOn = true;
	bool collidersOn = false;

	

	IGraphics* LoadGraphicSystem(EGraphicsSystem which)
	{
		switch (which)
		{
		case EGraphicsSystem::eHAPI:
			return new Visualisation();
			break;
		case EGraphicsSystem::eOpenGL:
			return new Visualisation();
			break;
		default:
			return new Visualisation();
			break;
		}

		return nullptr;
	}




	//Button commands
	//void ProcessButtonQuery


public:

	~World()
	{
		m_entities.clear();
		m_allInputHandlers.clear();
	}

	void LoadLevel();
	void Update();
	eGameReturnQ Run();

	//test
	void LoadMainMenu();
	

};



