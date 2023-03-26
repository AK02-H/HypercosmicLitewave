#pragma once

#include <string>
#include <cassert>
#include "Visualisation.h"	//include IGraphics instead?	//experiment later
#include "Vector2.h"
#include "NGLOBALS.h"


enum class ECollisionLayer
{
	eNeutral,
	ePlayer,
	eEnemy
};

class Entity
{
private:
	int entityId;
protected:
	std::string m_gfxName;
	float m_posX{ 0 }, m_posY{ 0 };	//why floats? try chaning later

	int dbgSz{ 60 };	//no longer needed
	
	//visualisation pointer so entities can control their own animation?(possibly only have this in certain entities like the player)

	virtual Entity* RetrieveEntity(std::vector<Entity*>* list, int rangeA, int rangeB, bool activeOnRetrieve = false);

public:

	bool isActive{ false };
	int id = 0;
	virtual void Load() = 0;				//Mention how Load() was barely used because most things were set up manually

	virtual void Update(float dt) = 0;
	virtual void Render(IGraphics& viz);
	virtual void LateUpdate(float dt) = 0;	//UPDATE AFTER RENDER
	//Does late update need to be pure virtual?


	virtual void SetPosition(float x, float y)
	{
		m_posX = x;
		m_posY = y;
	}

	Vector2 GetPosition() const
	{
		return Vector2{ m_posX, m_posY };
	}

	//setter function does not really need to exist. (?)
	virtual void SetActive(bool active) { isActive = active; }

	//For sound panning
	float RelativeScreenX();
	
	
};

