#pragma once
#include "Ent_Enemy.h"
class Ent_Enemy_Kamikaze :
    public Ent_Enemy
{
private:

	const float k_kamikazeSpeed = 800;
	const float k_timeUntilSuicide = 2.3f;
	float timer_suicide = 0;
	bool suicideState = false;

public:

	Ent_Enemy_Kamikaze(std::string gfxName) : Ent_Enemy(gfxName)
	{
		animator.SetFPS(FR_ENEMY_KAMIKAZE);
	}

	//Ensure that waves that involve kamikazes have a retreat time long enough
	//to accomodate for the easiest difficulty (x1.5)
	void Update(float dt) override final;

	void SetupEnemy(Vector2 position, Vector2 velocity, Vector2 acceleration, float fireDelay = 0) override;

	void KillMyself();
	
};

