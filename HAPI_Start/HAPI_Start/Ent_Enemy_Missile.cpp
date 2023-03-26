#include "Ent_Enemy_Missile.h"
#include "Ent_Player.h"

class Explosion;

void Ent_Enemy_Missile::Update(float dt)
{
	if (!hasExploded) Ent_Enemy::Update(dt);

	if (hasExploded)
	{
		timer_A += dt;

		if (timer_A > k_timeBetweenExplosion * LEVEL_SCALE)
		{
			//Move explosion position up
			//Spawn explosion effect
			//Move hitbox up

			explosionPositionA.y -= k_explosionMoveStep;
			movingExplosionA.Translate(0, -k_explosionMoveStep);
			timer_A = 0;

			Ent_Explosion* explosion = CAST_EXPLOSION(RetrieveEntity(all_entities, EXPLOSION_RANGE, false));

			if (explosion != nullptr)
			{
				//std::cout << "SPAWNED EXPLOSION" << std::endl;
				explosion->SetPosition(explosionPositionA.x, explosionPositionA.y);
				explosion->SetActive(true);
			}

			if (audioManager != nullptr)
			{
				audioManager->PlaySound(eSFX::eExplosion_1, true, 0.2f, RelativeScreenX());
			}

		}


		timer_B += dt;

		if (timer_B > k_timeBetweenExplosion * LEVEL_SCALE)
		{
			//Move explosion position up
			//Spawn explosion effect
			//Move hitbox up

			explosionPositionB.y -= k_explosionMoveStep;
			movingExplosionB.Translate(0, -k_explosionMoveStep);
			timer_B = 0;

			Ent_Explosion* explosion = CAST_EXPLOSION(RetrieveEntity(all_entities, EXPLOSION_RANGE, false));

			if (explosion != nullptr)
			{
				//std::cout << "SPAWNED EXPLOSION" << std::endl;
				explosion->SetPosition(explosionPositionB.x, explosionPositionB.y);
				explosion->SetActive(true);
			}

			if (audioManager != nullptr)
			{
				audioManager->PlaySound(eSFX::eExplosion_1, true, 0.2f, RelativeScreenX());
			}


		}

		timer_C += dt;

		if (timer_C > k_timeBetweenExplosion * LEVEL_SCALE)
		{
			//Move explosion position up
			//Spawn explosion effect
			//Move hitbox up

			explosionPositionC.y -= k_explosionMoveStep;
			movingExplosionC.Translate(0, -k_explosionMoveStep);
			timer_C = 0;

			Ent_Explosion* explosion = CAST_EXPLOSION(RetrieveEntity(all_entities, EXPLOSION_RANGE, false));

			if (explosion != nullptr)
			{
				//std::cout << "SPAWNED EXPLOSION" << std::endl;
				explosion->SetPosition(explosionPositionC.x, explosionPositionC.y);
				explosion->SetActive(true);
			}

			if (audioManager != nullptr)
			{
				audioManager->PlaySound(eSFX::eExplosion_1, true, 0.2f, RelativeScreenX());
			}




			if (explosionPositionC.y < k_highestYlimit)
			{
				hasExploded = false;
				isActive = false;
			}

		}
	}
	else
	{
		if (m_posY > k_detonationYPosition)
		{
			hasExploded = true;

			explosionPositionA = { m_posX + k_xOffsetA, m_posY };
			movingExplosionA = Rectangle(m_posX + k_xOffsetA, m_posX + k_xOffsetA + k_hbSize, m_posY, m_posY + k_hbSize);

			explosionPositionB = { m_posX + k_xOffsetB, m_posY + k_explosionMoveStep };
			movingExplosionB = Rectangle(m_posX + k_xOffsetB, m_posX + k_xOffsetB + k_hbSize, m_posY, m_posY + k_hbSize + k_explosionMoveStep * 1.5f);

			explosionPositionC = { m_posX + k_xOffsetC, m_posY + k_explosionMoveStep };
			movingExplosionC = Rectangle(m_posX + k_xOffsetC, m_posX + k_xOffsetC + k_hbSize, m_posY, m_posY + k_hbSize + k_explosionMoveStep * 1.5f);

			//std::cout << "MISSILE DETONATE" << std::endl;

			//Launch some light bullets

			/*
			x = cos(angle)
			y = sin(angle)
			*/

			for (size_t i = 0; i < 360; i += 20 * LEVEL_SCALE)	//find a way to only do certain arc
			{

				Vector2 newPosOnCircle = { (float)cos(i), (float)sin(i) };


				Ent_Bullet* bullet = CAST_BULLET(RetrieveEntity(all_entities, BULLETS_LIGHT_RANGE, true));
				bullet->PrimeBullet(m_posX, m_posY, newPosOnCircle.x, newPosOnCircle.y, 175);
			}

			audioManager->PlaySound(eSFX::eNuke, true, 1.0f, RelativeScreenX());

		}
	}
}

void Ent_Enemy_Missile::LateUpdate(float dt)
{
	Ent_Enemy::LateUpdate(dt);
	if (DEBUG_COLLIDERS)
	{
		explosionHbA.DebugDraw();
		explosionHbB.DebugDraw();
		explosionHbC.DebugDraw();
	}
}

void Ent_Enemy_Missile::CollisionCheck(std::vector<Entity*>* listOfEntities)
{
	Ent_Enemy::CollisionCheck(listOfEntities);

	if (!hasExploded) return;

	//Check each hb against players
	for (POOL_PLAYER_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;

		//Smelly
		Ent_Player* collidedObject = CAST_PLAYER((*listOfEntities)[i]);

		if (explosionHbA.Overlaps(&collidedObject->GetCollider()))
		{
			collidedObject->Die();
		}

		if (explosionHbB.Overlaps(&collidedObject->GetCollider()))
		{
			collidedObject->Die();
		}

		if (explosionHbC.Overlaps(&collidedObject->GetCollider()))
		{
			collidedObject->Die();
		}
	}
}

void Ent_Enemy_Missile::SpawnDeathExplosion()
{
	int adderX = 25;
	int adderY = 0;
	for (size_t i = 0; i < 6; i++)
	{
		Ent_Explosion* explosion = CAST_EXPLOSION(RetrieveEntity(all_entities, EXPLOSION_RANGE, false));

		if (explosion != nullptr)
		{
			//std::cout << "SPAWNED EXPLOSION" << std::endl;
			explosion->SetPosition(m_posX + adderX, m_posY + adderY);
			explosion->SetActive(true);
			explosion->SetSpriteEffect(SpriteEffect::eNegative);

		}




		adderY += 65;
	}

	if (audioManager != nullptr)
	{
		audioManager->PlaySound(eSFX::eNukeDie, true, 1.0f, RelativeScreenX());
	}
}
