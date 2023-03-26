#include "Ent_Enemy_Basic.h"
#include "Ent_Player.h"

void Ent_Enemy_Basic::Update(float dt)
{
	Ent_Enemy::Update(dt);

	//Attacking

	timer_fireInterval += dt;

	if (timer_fireInterval > fireInterval)
	{
		FireBullet();
		timer_fireInterval = 0;

	}
}

void Ent_Enemy_Basic::FireBullet()
{
	playerPtr = FindClosestPlayer();
	if (playerPtr == nullptr) return;
	Vector2 playerPos = playerPtr->GetPosition() + playerPtr->GetCenterOffset();
	Vector2 myPos = Vector2(m_posX, m_posY); // + GetCenterOffset();

	Vector2 direction = playerPos - myPos;
	direction.NormaliseInPlace();

	Ent_Bullet* bullet = nullptr;

	if (!fireLightBullet)
	{
		bullet = CAST_BULLET(RetrieveEntity(all_entities, BULLETS_ENEMY_A_RANGE, true));

	}
	else
	{
		bullet = CAST_BULLET(RetrieveEntity(all_entities, BULLETS_LIGHT_RANGE, true));
	}

	bullet->PrimeBullet(m_posX, m_posY, direction.x, direction.y, 450);

	if (audioManager != nullptr && IsInBounds()) audioManager->PlaySound(eSFX::eEnemyFire, true, 1.0f, RelativeScreenX());
}
