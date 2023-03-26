#include "Ent_Enemy.h"

#include "Ent_Player.h"
#include "Ent_PlayerSaber.h"
#include "Ent_Explosion.h"
#include "Ent_EnemyManager.h"	//Very dangerous circular dependency, get rid of if possible
#include "Ent_HUDManager.h"
#include "Ent_AudioManager.h"

//class Ent_EnemyManager;

//what does putting const after a function do
void Ent_Enemy::Update(float dt)
{
	//Moving

	SetPosition(m_posX + velocity.x * dt, m_posY + velocity.y * dt);
	velocity = velocity + acceleration * dt;
	
	if (m_posX < 0) m_posX = m_posX;
	

	


	if (saberInvulnerability)
	{
		timer_damageInvulnerabilityTime += dt;
		if (timer_damageInvulnerabilityTime > k_damageInvulnerabilityTime)
		{
			saberInvulnerability = false;
			timer_damageInvulnerabilityTime = 0;
		}
	}




	//Other
	if (isDamageFlashing)
	{
		timer_damageFlashTime += dt;

		if (timer_damageFlashTime > k_damageFlashTime)
		{
			isDamageFlashing = false;
			timer_damageFlashTime = 0;
		}
	}


	//Animate
	animator.Animate(dt);
}

void Ent_Enemy::Render(IGraphics& viz)
{
	//have if check that checks whether it's animated or not, otherwise use regular render


	if(!isDamageFlashing)
	{
		viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame);
	}
	else
	{
 		viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame, SpriteEffect::eFlashWhitePartial);
	}
}


void Ent_Enemy::LateUpdate(float dt)
{
	if (DEBUG_COLLIDERS) collider.DebugDraw();
}

void Ent_Enemy::SetPosition(float x, float y)
{
	m_posX = x;
	m_posY = y;

	collisionBox.left = m_posX + (int)hboxOffset.x;
	collisionBox.right = m_posX + collisionBox.permaWidth + (int)hboxOffset.x;
	collisionBox.top = m_posY + (int)hboxOffset.y;
	collisionBox.bottom = m_posY + collisionBox.permaHeight + (int)hboxOffset.y;
}

void Ent_Enemy::CollisionCheck(std::vector<Entity*>* listOfEntities)
{
	if (!isActive) return; //just in case

	//range in pool for player bullets
	for (POOL_BULLETSPLAYER_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;

		//Smelly
		Ent_Bullet* collidedObject = CAST_BULLET((*listOfEntities)[i]);

		if (collider.Overlaps(collidedObject->GetCollider()))
		{

			TakeDamage(collidedObject->GetDamage());

			(*listOfEntities)[i]->SetActive(false);
		}
	}

	if (isMultiplayerMode)
	{
		for (POOL_BULLETSPLAYER_B_LOOP)
		{
			if (!(*listOfEntities)[i]->isActive) continue;

			//Smelly
			Ent_Bullet* collidedObject = CAST_BULLET((*listOfEntities)[i]);

			if (collider.Overlaps(collidedObject->GetCollider()))
			{

				TakeDamage(collidedObject->GetDamage());

				(*listOfEntities)[i]->SetActive(false);
			}
		}
	}

	//Check for collision with saber
	for (POOL_SABER_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;

		bool beenHitBySaber = false;


		//Smelly
		Ent_PlayerSaber* collidedObject = CAST_SABER((*listOfEntities)[i]);
		std::vector<Collider*> saberBoxes = collidedObject->GetHitboxes();

		for (Collider*& hbox : saberBoxes)
		{

			if (collider.Overlaps(hbox))
			{
				std::cout << "enemy collided with saber at " << HAPI.GetTime() << ", saber ID: " << i << std::endl;

				TakeDamage(collidedObject->GetDamage());
				//Once hit by saber do not check for more collisions to prevent
				//taking damage from multiple hitboxes
				beenHitBySaber = true;
				saberInvulnerability = true;
				break;
				//(*listOfEntities)[i]->SetActive(false);
			}
		}

		if (beenHitBySaber)
		{
			break;
		}

	}

	//check for collision with reflected light bullets
	for (POOL_BULLETS_LIGHT_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;



		//Smelly
		Ent_Bullet_Light* collidedObject = CAST_BULLET_LIGHT((*listOfEntities)[i]);

		if (!collidedObject->GetHasBeenDeflected()) continue;

		//make sure to prevent damage being taken from the same bullet more than once
		if (collider.Overlaps(collidedObject->GetCollider()))
		{


			TakeDamage(collidedObject->GetDamage(), true);
			//saberInvulnerability = true;	//light bullet invulnerability period?

			break;

		}

	}
}

void Ent_Enemy::SetupEnemy(Vector2 position, Vector2 velocity, Vector2 acceleration, float fireDelay)
{
	declaredDead = false;

	m_hp = m_maxHp;

	m_posX = position.x;
	m_posY = position.y;

	this->velocity = velocity;
	this->acceleration = acceleration;

	timer_fireInterval = 0 - fireDelay;

}

void Ent_Enemy::SetupPlayerList()
{
	for(POOL_PLAYER_LOOP)

	{
		//all_players.push_back(CAST_PLAYER((*all_entities)[i]));
		//Ent_Player* p = static_cast<Ent_Player*>((*all_entities)[i]);
		Ent_Player* p = static_cast<Ent_Player*>((*all_entities)[i]);
		if (p != nullptr)all_players.push_back(p);
	}

	/*all_players.push_back(CAST_PLAYER((*all_entities)[PER_BULLETSPLAYER_START]));
	all_players.push_back(CAST_PLAYER((*all_entities)[PER_BULLETSPLAYER_START+1]));*/
}

Ent_Player* Ent_Enemy::FindClosestPlayer()
{
	Vector2 thisPosition{ m_posX, m_posY };
	float minDist = INFINITY;
	Ent_Player* nearestPlayer = nullptr;

	int t = 0;
	for (Ent_Player* player : all_players)
	{
		t++;

		if (!player->isActive) continue;
		Vector2 newPos = player->GetPosition();
		float dist = abs((newPos - thisPosition).Length());
		std::cout << "DIST TO P " << t << " IS " << dist << std::endl;


		if (dist < minDist)
		{
			nearestPlayer = player;
			minDist = dist;
		}
	}

	return nearestPlayer;

}

void Ent_Enemy::TakeDamage(int damage, bool wasFromLight)
{
	if (!isActive) return;
	if (saberInvulnerability) return;
	if (declaredDead) return;
	//std::cout << "Enemy take " << damage << " damage" << std::endl;

	m_hp -= damage;

	
	timer_damageFlashTime = 0;
	isDamageFlashing = true;

	if (m_hp <= 0)
	{
		//die
		declaredDead = true;

		//spawn explosion

		SpawnDeathExplosion();

		int scoreValue = m_scoreValue;

		if (wasFromLight) scoreValue *= LIGHTBULLET_SCOREMULTIPLIER;

		if (hudManager != nullptr)
		{
			std::cout << " TEXT CALLED ON " << id << std::endl;
			if (!wasFromLight)
			{
				hudManager->CreatePopupText(std::to_string(scoreValue), { m_posX, m_posY }, 1.0f, UI_TEXTCOL_BRIGHT, 30);

			}
			else
			{
				hudManager->CreatePopupText(std::to_string(scoreValue), { m_posX, m_posY }, 1.0f, UI_TEXTCOL_LIGHT, 40);

			}
		}

		//add score
		if (scorePtr != nullptr)
		{
			*scorePtr += scoreValue;
		}


		//record death in enemy monitor list
		Ent_EnemyManager* e = static_cast<Ent_EnemyManager*>(RetrieveEntity(all_entities, PER_ENEMYMANAGER_START, PER_ENEMYMANAGER_END));
		e->IncrementDeathCount();
		//std::cout << "ENEMYID " << id << " DEATH" << std::endl;

		SetActive(false);
	}

	//saberInvulnerability = true;
}
//not used currently
void Ent_Enemy::Die()
{
	

}

void Ent_Enemy::SpawnDeathExplosion()
{
	Ent_Explosion* explosion = CAST_EXPLOSION(RetrieveEntity(all_entities, EXPLOSION_RANGE, false));

	if (explosion != nullptr)
	{
		//std::cout << "SPAWNED EXPLOSION" << std::endl;
		explosion->SetPosition(m_posX, m_posY);
		explosion->SetActive(true);
	}

	if (audioManager != nullptr)
	{
		audioManager->PlaySound(eSFX::eExplosion_1, true, 1.0f, RelativeScreenX());
	}

	if (hudManager != nullptr)
	{
		hudManager->CreatePopupText(std::to_string(m_scoreValue), { m_posX, m_posY }, 1.0f);
	}
}

void Ent_Enemy::FireBullet()
{
	//assert retrieve not returning null

	Ent_Bullet* bullet = CAST_BULLET(RetrieveEntity(all_entities, BULLETS_ENEMY_A_RANGE, true));
	bullet->PrimeBullet(m_posX + fireOffset.x, m_posY + fireOffset.y, 0, 1, 600);

	if (audioManager != nullptr && IsInBounds()) audioManager->PlaySound(eSFX::eEnemyFire, true, 1.0f, RelativeScreenX());
}

void Ent_Enemy::HandleBullets()
{
	if (counterTime1 > bulletTime1 && counterWave1 < 1)
	{
		if (numBullet1 == maxBullets)
		{
			numBullet1 = 0;
			counterWave1++;
		}


		float x = 0;
		float y = 0;

		x = r * cos((size_t)(numBullet1 % 180) / PI);
		y = r * sin((size_t)(numBullet1 % 30) / PI);



		//std::cout << "X " << x << std::endl;
		//std::cout << "Y " << y << std::endl;

		Vector2 dir(x, y);
		dir.NormaliseInPlace();
		Ent_Bullet* bullet = static_cast<Ent_Bullet*>(RetrieveEntity(all_entities, BULLETS_ENEMY_A_RANGE, true));
		bullet->PrimeBullet(m_posX + fireOffset.x + x, m_posY + fireOffset.y + y, dir.x, dir.y, 300);


		numBullet1++;
		counterTime1 = counterTime1 - bulletTime1;


	}
}

bool Ent_Enemy::IsInBounds()
{
	if (m_posX > ENEMYSCREENBOUND_MAX_X || m_posX < ENEMYSCREENBOUND_MIN_X || m_posY > ENEMYSCREENBOUND_MAX_Y || m_posY < ENEMYSCREENBOUND_MIN_Y)
	{
		return false;
	}
	else
	{
		return true;
	}
}
