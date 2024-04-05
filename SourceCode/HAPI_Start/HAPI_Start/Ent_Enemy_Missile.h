#pragma once
#include "Ent_Enemy.h"

class Ent_Enemy_Missile :
	public Ent_Enemy
{
private:

	const float k_detonationYPosition = SCR_HEIGHT;
	const float k_highestYlimit = -80;

	const float k_timeBetweenExplosion = 0.03f;
	const float k_explosionMoveStep = 60;
	const float k_hbSize = 60;


	const int k_xOffsetA = 30;
	Vector2 explosionPositionA = { 0, 0 };
	Rectangle movingExplosionA{ 0, 0, 0, 0 };
	Collider explosionHbA{ &movingExplosionA };
	float timer_A = k_timeBetweenExplosion / 2;

	const int k_xOffsetB = -10;
	Vector2 explosionPositionB = { 0, 0 };
	Rectangle movingExplosionB{ 0, 0, 0, 0 };
	Collider explosionHbB{ &movingExplosionB };
	float timer_B = 0;

	const int k_xOffsetC = 80;
	Vector2 explosionPositionC = { 0, 0 };
	Rectangle movingExplosionC{ 0, 0, 0, 0 };
	Collider explosionHbC{ &movingExplosionC };
	float timer_C = 0;


	

	bool hasExploded = false;



public:

	Ent_Enemy_Missile(std::string gfxName) : Ent_Enemy(gfxName)
	{
		animator.SetFPS(FR_ENEMY_MISSILE);

		timer_A = k_timeBetweenExplosion / 2;
		timer_B = 0;
		timer_C = 0;
	}

	void Update(float dt) override final;
	

	void Render(IGraphics& viz) override final
	{
		if (hasExploded) return;
		Ent_Enemy::Render(viz);
	}

	void LateUpdate(float dt) override final;
	

	void CollisionCheck(std::vector<Entity*>* listOfEntities) override final;
	
	void SpawnDeathExplosion() override final;
	
};

