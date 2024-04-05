#pragma once
#include "Ent_Enemy.h"
class Ent_Enemy_Vertical :
    public Ent_Enemy
{
private:
	
	const float k_volleyInterval = 0.015f;
	float timer_volley{ 0 };

	const int k_maxVolley = 10;
	int volleyCounter = 0;

public:

	Ent_Enemy_Vertical(std::string gfxName) : Ent_Enemy(gfxName)
	{
		
	}

	void Update(float dt) override;
	
};

