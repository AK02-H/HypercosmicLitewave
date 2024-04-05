#include "Ent_Enemy_Vertical.h"

void Ent_Enemy_Vertical::Update(float dt)
{
	Ent_Enemy::Update(dt);

	//Attacking

	timer_fireInterval += dt;

	if (timer_fireInterval > fireInterval)
	{

		timer_volley += dt;

		if (timer_volley > k_volleyInterval)
		{
			FireBullet();
			volleyCounter++;
			timer_volley = 0;

			if (volleyCounter >= k_maxVolley)
			{
				volleyCounter = 0;
				timer_fireInterval = 0;
			}
		}
	}
}
