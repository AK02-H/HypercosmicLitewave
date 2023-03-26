#include "Ent_Player.h"

void Ent_Player::Load()
{
	//initialise collider default position
	//m_posX = 150;
}

//add code that resets the player position if they ever go out of bounds during a hiccup, or clamp their position
void Ent_Player::Update(float dt)
{
	//none of my asserts are working
	assert(all_entities != nullptr && "WARNING: ENTITY POINTER NOT SET IN PLAYER");
	assert(inputter != nullptr && "WARNING: INPUT HANDLER NOT SET FOR PLAYER");
	assert(bulletSaber != nullptr && "WARNING: BULLET SABER NOT SET FOR PLAYER");


	//skip update if temporarily dead

	if (tempDeath)
	{
		timer_respawn += dt;
		if (timer_respawn > k_respawnTime)
		{
			timer_respawn = 0;
			Respawn();
		}
		else
		{
			return;
		}
	}



	animator.Animate(dt);
	//there are some currently issues with input 'locking', where holding down a combination of certain
	// keys, always involving the right key, prevents any inputs from changing
	//Move sprites
	
	velocity.x = 0;
	velocity.y = 0;

	if (inputter->GetInputHold(InputMap::eLeft))
		velocity.x -= 1;
	if (inputter->GetInputHold(InputMap::eRight))
		velocity.x += 1;
	if (inputter->GetInputHold(InputMap::eUp))
		velocity.y -= 1;
	if (inputter->GetInputHold(InputMap::eDown))
		velocity.y += 1;

	//std::cout << "VELOCITY IS X " << velocity.x << " Y " << velocity.y << std::endl;
	//std::cout << "DELTA IS " << dt << std::endl;


	//Handle Animations with velocity values first
	if (velocity.x < 0)	//LEFT
	{
		if (velocity.y > 0) //DOWN
		{
			animator.SetAnimation(*vizPtr, "Left_Back", true, true);
		}
		else if (velocity.y < 0) //UP
		{
			animator.SetAnimation(*vizPtr, "Left_Forw", true, true);
		}
		else
		{
			animator.SetAnimation(*vizPtr, "Left", true, true);
		}
	}
	else if (velocity.x > 0) //RIGHT
	{
		if (velocity.y > 0) //DOWN
		{
			animator.SetAnimation(*vizPtr, "Right_Back", true, true);
		}
		else if (velocity.y < 0) //UP
		{
			animator.SetAnimation(*vizPtr, "Right_Forw", true, true);
		}
		else
		{
			animator.SetAnimation(*vizPtr, "Right", true, true);
		}
	}
	else if (velocity.y > 0) //DOWN
	{
		if (velocity.x > 0) //LEFT
		{
			animator.SetAnimation(*vizPtr, "Left_Back", true, true);
		}
		else if (velocity.x < 0) //RIGHT
		{
			animator.SetAnimation(*vizPtr, "Right_Back", true, true);
		}
		else
		{
			animator.SetAnimation(*vizPtr, "Backward", true, true);
		}
	}
	else if (velocity.y <  0) //UP
	{
		if (velocity.x > 0) //LEFT
		{
			animator.SetAnimation(*vizPtr, "Left_Forw", true, true);
		}
		else if (velocity.x < 0) //RIGHT
		{
			animator.SetAnimation(*vizPtr, "Right_Forw", true, true);
		}
		else
		{
			animator.SetAnimation(*vizPtr, "Forward", true, true);
		}
	}
	else if (velocity.x == 0 && velocity.y == 0)
	{
		animator.SetAnimation(*vizPtr, "Neutral", true, true);
	}

	velocity.NormaliseInPlace();

	velocity.x *= m_speed * dt;
	velocity.y *= m_speed * dt;

	
	//Make this function more like displacement?
	SetPosition((int)m_posX + (int)velocity.x, (int)m_posY + (int)velocity.y);


	//also move saber
	if (bulletSaber != nullptr)
	{
		bulletSaber->UpdatePosition(m_posX, m_posY);
	}


	


	/*if (m_keyData.scanCode[HK_LEFT] || m_keyData.scanCode['A'] || ((m_controllerData.isAttached) ? m_controllerData.digitalButtons[HK_DIGITAL_DPAD_LEFT] : false))
		dx -= magnitude;
	if (m_keyData.scanCode[HK_RIGHT] || m_keyData.scanCode['D'] || ((m_controllerData.isAttached) ? m_controllerData.digitalButtons[HK_DIGITAL_DPAD_RIGHT] : false))
		dx += magnitude;
	if (m_keyData.scanCode[HK_UP] || m_keyData.scanCode['W'] || ((m_controllerData.isAttached) ? m_controllerData.digitalButtons[HK_DIGITAL_DPAD_UP] : false))
		dy -= magnitude;
	if (m_keyData.scanCode[HK_DOWN] || m_keyData.scanCode['S'] || ((m_controllerData.isAttached) ? m_controllerData.digitalButtons[HK_DIGITAL_DPAD_DOWN] : false))
		dy += magnitude;*/


	if (!canFire)
	{
		timer_fireInterval += dt;
		if (timer_fireInterval >= k_fireInterval)
		{
			canFire = true;
			timer_fireInterval = 0;
		}
	}

	if (inputter->GetInputHold(InputMap::eFire))
	{

		if (canFire)
		{		
			FireBullet();
			canFire = false;
		}
	}

	if (inputter->GetInputDown(InputMap::eFireB))
	{

		//if (bulletSaber->GetAnimator()->GetCurrentAnimationName() != "Slash_Hyper")
		if (bulletSaber->prep)
		{
			std::cout << "Saber" << std::endl;
			bulletSaber->SetActive(true);
			bulletSaber->Slash(inputter);
			bulletSaber->StartCharging();

			inputter->InvokeRumble(10000, 4000, 0.3f);
		}

		
	}

	//If get input up, and saber is in charging state
	if (inputter->GetInputUp(InputMap::eFireB))
	{
		bulletSaber->ReleaseCharge(inputter);
	}

	//flash while charging to indicate charge state
	if (bulletSaber->GetIsCharging())
	{

		timer_chargeFlash += dt;

		float interval = 0;

		if (bulletSaber->GetIsAtHyperTime())
		{
			interval = k_flashIntervalHyper;
		}
		else
		{
			interval = k_flashIntervalRegular;
		}
		if (timer_chargeFlash > interval)
		{
			isChargeFlash = !isChargeFlash;
			timer_chargeFlash = 0;
		}


	}
	else
	{
		isChargeFlash = false;
		timer_chargeFlash = 0;

	}

	//recovery frames flash
	if (recoveryState)
	{
		timer_recoveryTime += dt;
		timer_recoveryFlashInterval += dt;

		if (timer_recoveryFlashInterval > k_recoveryFlashTime)
		{
			isRecoveryFlash = !isRecoveryFlash;
			timer_recoveryFlashInterval = 0;
		}

		if (timer_recoveryTime > k_maxRecoveryTime)
		{
			recoveryState = false;
			isRecoveryFlash = false;
			timer_recoveryTime = false;
			timer_recoveryFlashInterval = 0;
		}


	}

	
	//m_posX--;


}

void Ent_Player::Render(IGraphics& viz)
{
	if (tempDeath) return;

	if (!isRecoveryFlash)
	{
		viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame, (!isChargeFlash) ? SpriteEffect::eNone : SpriteEffect::eNegative);
	}
	else
	{
		viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame, SpriteEffect::eQuarterAlpha);
	}

}

void Ent_Player::LateUpdate(float dt)
{
	if (DEBUG_COLLIDERS) collider.DebugDraw();
}

void Ent_Player::CollisionCheck(std::vector<Entity*>* listOfEntities)
{
	if (!isActive) return; //just in case

	//range in pool for player bullets
	for (POOL_BULLETS_ENEMY_A_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;

		//Smelly
		Ent_Bullet* collidedObject = CAST_BULLET((*listOfEntities)[i]);

		if (collider.Overlaps(collidedObject->GetCollider()))
		{
			std::cout << "player collided with bullet at " << HAPI.GetTime() << std::endl;

			Die();

			(*listOfEntities)[i]->SetActive(false);
		}
	}


	//range in pool for player bullets
	for (POOL_BULLETS_LIGHT_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;

		//Smelly
		Ent_Bullet_Light* collidedObject = CAST_BULLET_LIGHT((*listOfEntities)[i]);

		if (collidedObject->GetHasBeenDeflected()) continue;

		if (collider.Overlaps(collidedObject->GetCollider()))
		{
			std::cout << "player collided with bullet at " << HAPI.GetTime() << std::endl;

			Die();

			//(*listOfEntities)[i]->SetActive(false);
		}
	}

	//Against enemies

	for (POOL_ENEMY_ALL_LOOP)
	{
		if (!(*listOfEntities)[i]->isActive) continue;

		//Smelly
		Ent_Enemy* collidedObject = CAST_ENEMY((*listOfEntities)[i]);

		if (collider.Overlaps(collidedObject->GetCollider()))
		{
			std::cout << "player collided with enemy at " << HAPI.GetTime() << std::endl;



			Die();

			//(*listOfEntities)[i]->SetActive(false);
		}
	}

}

void Ent_Player::SetPosition(float x, float y)
{

	if (x < PLAYER_MIN_X)
	{
		x = PLAYER_MIN_X;
	}
	else if (x > PLAYER_MAX_X)
	{
		x = PLAYER_MAX_X;
	}

	if (y < PLAYER_MIN_Y)
	{
		y = PLAYER_MIN_Y;
	}
	else if (y > PLAYER_MAX_Y)
	{
		y = PLAYER_MAX_Y;
	}

	m_posX = x;
	m_posY = y;


	

	//this code is the source of why colliders shrink when in the negative coordinates, definitely
	int width = collisionBox.Width();
	int height = collisionBox.Height();

	//   collisionBox.t
	//change use of debug size later
	collisionBox.left = m_posX + 51;
	collisionBox.right = m_posX + width + 51;
	collisionBox.top = m_posY + 38;
	collisionBox.bottom = m_posY + height + 38;

	// OffsetCollider(48, 21);
}

void Ent_Player::FireBullet()
{
	//assert retrieve not returning null

	Ent_Bullet* bullet = nullptr;
	Ent_Bullet* bullet2 = nullptr;
	Ent_Bullet* bullet3 = nullptr;

	if (!isSecondPlayer)
	{
		bullet = CAST_BULLET(RetrieveEntity(all_entities, BULLETSPLAYER_RANGE, true));
		bullet2 = CAST_BULLET(RetrieveEntity(all_entities, BULLETSPLAYER_RANGE, true));
		bullet3 = CAST_BULLET(RetrieveEntity(all_entities, BULLETSPLAYER_RANGE, true));
	}
	else
	{
		bullet = CAST_BULLET(RetrieveEntity(all_entities, BULLETSPLAYER_B_RANGE, true));
		bullet2 = CAST_BULLET(RetrieveEntity(all_entities, BULLETSPLAYER_B_RANGE, true));
		bullet3 = CAST_BULLET(RetrieveEntity(all_entities, BULLETSPLAYER_B_RANGE, true));
	}
	
	if (bullet != nullptr) bullet->PrimeBullet(m_posX + 30, m_posY - 0, 0, -1);
	if (bullet2 != nullptr) bullet2->PrimeBullet(m_posX + 70, m_posY - 0, 0, -1);
	if (bullet3 != nullptr) bullet3->PrimeBullet(m_posX + 50, m_posY - 0, 0, -1);


	if (audioManager != nullptr)
	{
		float rel = RelativeScreenX();
		audioManager->PlaySound(eSFX::ePlayerFire, true, 0.3f, rel);
	}

	inputter->InvokeRumble(9000, 9000, 0.05f);

}

void Ent_Player::Die()
{
	if (recoveryState) return;	//no death

	//This should change depending on whether it's a 1p or 2p game, but I'll leave for now
	Chron::GetInstance().timescale = Chron::GetInstance().timeBASE;


	Ent_Explosion* explosion = CAST_EXPLOSION(RetrieveEntity(all_entities, EXPLOSION_TYPEB_RANGE, false));

	if (explosion != nullptr)
	{
		std::cout << "SPAWNED EXPLOSION" << std::endl;
		explosion->SetPosition(m_posX, m_posY);
		explosion->SetActive(true);
	}

	if (audioManager != nullptr)
	{
		audioManager->PlaySound(eSFX::ePlayerDie, true, 1.0f, RelativeScreenX());
	}

	bulletSaber->ResetSaber();

	recoveryState = true;
	isRecoveryFlash = true;
	timer_recoveryTime = 0;
	timer_recoveryFlashInterval = 0;

	tempDeath = true;
}

void Ent_Player::Respawn()
{
	if (m_lifeCount <= 0)
	{
		//Do not respawn
		isActive = false;

		//True 'death' (allows for delay between each death so things don't happen instantly)
		*deathMonitor = true;
	}
	else
	{
		m_lifeCount--;
		tempDeath = false;

		if (audioManager != nullptr) audioManager->PlaySound(eSFX::ePlayerRespawn, true, 1.0f, RelativeScreenX());

		if (!isSecondPlayer)
		{
			SetPosition(PLAYER1_SPAWNPOS);
		}
		else
		{
			SetPosition(PLAYER2_SPAWNPOS);
		}
	}


}
