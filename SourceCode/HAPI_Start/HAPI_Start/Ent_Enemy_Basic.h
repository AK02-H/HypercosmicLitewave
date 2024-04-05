#pragma once
#include "Ent_Enemy.h"

class Ent_Enemy_Basic :
    public Ent_Enemy
{
public:

	bool fireLightBullet = false;

	Ent_Enemy_Basic(std::string gfxName) : Ent_Enemy(gfxName)
	{
		//playerPtr = CAST_PLAYER(RetrieveEntity(all_entities, PLAYER_RANGE));
	}

	void Update(float dt) override final;
    
	void FireBullet() override final;
	

};

