#include "Ent_PlayerSaber.h"
#include "InputHandler.h"

class InputHandler;

void Ent_PlayerSaber::BeginHitstop()
{
    //g_timescale = k_hitstopSlowRate;
    Chron::GetInstance().timescale = Chron::GetInstance().timeBASE * k_hitstopSlowRate;
    timer_hitstop = 0;
    hitstopActive = true;
}

void Ent_PlayerSaber::Update(float dt)
{
	animator.Animate(dt);

	if (animator.onCompleteSignal)
	{
        if (animator.GetCurrentAnimationName() == "Slash")
        {
            if (!isCharging)
            {
                SetActive(false);
            }
            initialAnimationEnded = true;
        }
        else if (animator.GetCurrentAnimationName() == "Slash_Hyper")
        {
            SetActive(false);
            hyperSlashState = false;
            //animator.SetAnimationFrame(0);
            //animator.SetAnimation(*vizPtr, "Slash");
            prep = true;
        }
        else
        {
            initialAnimationEnded = false;
            //hyperSlashState = false;
        }
	}
    else
    {
        initialAnimationEnded = false;

    }

    if (initialAnimationEnded && !isCharging)
    {
        SetActive(false);
    }

    if (!isCharging && animator.frame == 8 && animator.GetCurrentAnimationName() == "Slash")
    {
        SetActive(false);
    }

    if (hyperSlashState && animator.frame == 18 && animator.GetCurrentAnimationName() == "Slash_Hyper")
    {
        SetActive(false);
    }
	
    if (!hyperSlashState)
    {
        currentHboxA = animatedHitboxesA[animator.frame];
        currentHboxB = animatedHitboxesB[animator.frame];
        currentHboxC = animatedHitboxesC[animator.frame];
        currentHboxD = animatedHitboxesD[animator.frame];
    }
    else
    {
        currentHboxA = hyperAnimatedHitboxesA[animator.frame];
        currentHboxB = hyperAnimatedHitboxesB[animator.frame];
        currentHboxC = hyperAnimatedHitboxesC[animator.frame];
        currentHboxD = hyperAnimatedHitboxesD[animator.frame];
    }
	
    //Does this work properly? (might be cause of random damage issues)
	currentHboxA.Translate(m_posX, m_posY);
	currentHboxB.Translate(m_posX, m_posY);
	currentHboxC.Translate(m_posX, m_posY);
	currentHboxD.Translate(m_posX, m_posY);

    if (isCharging)
    {
        chargeTimer += dt;

        if(animator.frame == 8) stopCheckingCollisions = true;

        if (chargeTimer > k_timeUntilHyperCharged)
        {
            if (!hasPlayedChargeSound)
            {
                audioManager->PlaySound(eSFX::ePlayerChargeReady, true, 1.0f, RelativeScreenX());
                hasPlayedChargeSound = true;
            }
            //std::cout << "HYPER CHARGED" << std::endl;
            prep = false;
        }
    }

    if (hyperSlashState)
    {
        //std::cout << "HYPER STATE" << std::endl;
    }
    else
    {
        //std::cout << "NOT STATE" << std::endl;

    }


    //Manage hitstop slowdown
    if (hitstopActive)
    {
        timer_hitstop += dt / TIMESCALE; //division makes this still scale in real time
        if (timer_hitstop > k_hitstopDuration)
        {
            //revert time
            Chron::GetInstance().timescale = Chron::GetInstance().timeBASE;
            timer_hitstop = 0;
            hitstopActive = false;
        }

    }
}

void Ent_PlayerSaber::Render(IGraphics& viz)
{
	viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame);
}

void Ent_PlayerSaber::LateUpdate(float dt)
{
	if (DEBUG_COLLIDERS)
	{
        if (stopCheckingCollisions) return;

		hitboxA.DebugDraw();
		hitboxB.DebugDraw();
		hitboxC.DebugDraw();
		hitboxD.DebugDraw();
	};
}

const std::vector<Collider*> Ent_PlayerSaber::GetHitboxes()
{
    std::vector<Collider*> toReturn;
    toReturn.push_back(&hitboxA);
    toReturn.push_back(&hitboxB);
    toReturn.push_back(&hitboxC);
    toReturn.push_back(&hitboxD);
    return toReturn;
}

void Ent_PlayerSaber::UpdatePosition(float posX, float posY)
{
    if (!hyperSlashState)
    {
        m_posX = posX + offsetFromPlayer.x;
        m_posY = posY + offsetFromPlayer.y;
    }
    else
    {
        m_posX = posX + offsetFromPlayer_hyper.x;
        m_posY = posY + offsetFromPlayer_hyper.y;
    }

}

void Ent_PlayerSaber::Slash(InputHandler* rumblerPtr)
{
    if (hyperSlashState)
    {
        std::cout << "ATTEMPTED TO SLASH DURING HYPER" << std::endl;
        return;
    }
    else
    {

    }

    isActive = true;
    stopCheckingCollisions = false;
    initialAnimationEnded = false;
    animator.SetAnimation(*vizPtr, "Slash");
    animator.SetAnimationFrame(0);

    rumbler = rumblerPtr;

    if (audioManager != nullptr) audioManager->PlaySound(eSFX::ePlayerSlash, true, 1.0f, RelativeScreenX());
}

void Ent_PlayerSaber::StartCharging()
{
    chargeTimer = 0;
    hyperSlashState = false;
    isCharging = true;
    hasPlayedChargeSound = false;
}

void Ent_PlayerSaber::ReleaseCharge(InputHandler* rumbler)
{
    if (chargeTimer > k_timeUntilHyperCharged)
    {
        //Release hyper slash
        isActive = true;

        if (vizPtr != nullptr)
        {
            animator.SetAnimation(*vizPtr, "Slash_Hyper");
            if (audioManager != nullptr) audioManager->PlaySound(eSFX::ePlayerSlashHyper, true, 1.0f, RelativeScreenX());

        }
        animator.SetAnimationFrame(0);
        hyperSlashState = true;


        if (rumbler != nullptr) rumbler->InvokeRumble(5000, 22000, 1);
    }
    else
    {
        /*if (initialAnimationEnded)
        {
            SetActive(false);
        }
        SetActive(false);*/

    }

    isCharging = false;
    hasPlayedChargeSound = false;
    stopCheckingCollisions = false;
    chargeTimer = 0;
}

void Ent_PlayerSaber::ResetSaber()
{
    isCharging = false;
    hasPlayedChargeSound = false;
    stopCheckingCollisions = false;
    chargeTimer = 0;
    animator.SetAnimationFrame(0);
    initialAnimationEnded = false;
    hyperSlashState = false;
    animator.SetAnimation(*vizPtr, "Slash");
    prep = true;
    SetActive(false);
}

void Ent_PlayerSaber::CollisionCheck(std::vector<Entity*>* listOfEntities)
{
    if (!isActive) return; //just in case

    if (stopCheckingCollisions) return; //prevent ghost collisions while charging


    //for (POOL_ENEMY_ALL_LOOP)
    //{
    //    if (!(*listOfEntities)[i]->isActive) continue;

    //    bool beenHitBySaber = false;


    //    //Smelly
    //    Ent_Enemy* collidedObject = CAST_ENEMY((*listOfEntities)[i]);
    //    std::vector<Collider*> saberBoxes = GetHitboxes();

    //    for (Collider*& hbox : saberBoxes)
    //    {

    //        if (collidedObject->GetCollider()->Overlaps(hbox))
    //        {
    //            std::cout << "enemy collided with saber at " << HAPI.GetTime() << ", saber ID: " << i << std::endl;

    //            collidedObject->TakeDamage(GetDamage());
    //            //Once hit by saber do not check for more collisions to prevent
    //            //taking damage from multiple hitboxes
    //            beenHitBySaber = true;
    //            collidedObject->saberInvulnerability = true;
    //            break;
    //            //(*listOfEntities)[i]->SetActive(false);
    //        }
    //    }

    //    if (beenHitBySaber)
    //    {
    //        break;
    //    }

    //}




    std::vector< Ent_Bullet_Light*> toDeflect;

    //range in pool for player bullets
    for (POOL_BULLETS_LIGHT_LOOP)
    {
        if (!(*listOfEntities)[i]->isActive) continue;

        //Smelly
        Ent_Bullet_Light* collidedObject = CAST_BULLET_LIGHT((*listOfEntities)[i]);
        

        if (hitboxA.Overlaps(collidedObject->GetCollider()))
        {
            std::cout << "saber A collided with light bullet " << HAPI.GetTime() << std::endl;

            //Do something to prevent collision being detected more than once
            //Reflect bullet
            if (!collidedObject->GetHasBeenDeflected())
            {
                toDeflect.push_back(collidedObject);
                continue;
            }

        }
        else if (hitboxB.Overlaps(collidedObject->GetCollider()))
        {
            std::cout << "saber B collided with light bullet " << HAPI.GetTime() << std::endl;

            //Do something to prevent collision being detected more than once
            //Reflect bullet
            if (!collidedObject->GetHasBeenDeflected())
            {
                toDeflect.push_back(collidedObject);
                continue;
            }

        }
        else if (hitboxC.Overlaps(collidedObject->GetCollider()))
        {
            std::cout << "saber C collided with light bullet " << HAPI.GetTime() << std::endl;

            //Do something to prevent collision being detected more than once
            //Reflect bullet
            if (!collidedObject->GetHasBeenDeflected())
            {
                toDeflect.push_back(collidedObject);
                continue;
            }

        }
        else if (hitboxD.Overlaps(collidedObject->GetCollider()))
        {
            std::cout << "saber D collided with light bullet " << HAPI.GetTime() << std::endl;

            //Do something to prevent collision being detected more than once
            //Reflect bullet
            if (!collidedObject->GetHasBeenDeflected())
            {
                toDeflect.push_back(collidedObject);
                continue;
            }

        }
    }

    if (!toDeflect.empty())
    {
        for (Ent_Bullet_Light*& bullet : toDeflect)
        {
            bullet->DeflectBullet(k_deflectPower, hyperSlashState);
        }
        if(rumbler != nullptr) rumbler->InvokeRumble(20000, 20000, 0.1f);

        BeginHitstop();
        audioManager->PlaySound(eSFX::eDeflect, true, 1.0f, RelativeScreenX());
    }
    












}
