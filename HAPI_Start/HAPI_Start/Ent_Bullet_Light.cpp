#include "Ent_Bullet_Light.h"
#include "Ent_Player.h"

class Ent_Player;

void Ent_Bullet_Light::Render(IGraphics& viz)
{
    if (hasBeenDeflected)
    {
        viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame);
    }
    else
    {
        viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame, SpriteEffect::eNegative);

    }

}

void Ent_Bullet_Light::SetPlayerPtrAndDirection(Ent_Player* player)
{
    playerPtr = player;
    Vector2 playerPos = playerPtr->GetPosition() + playerPtr->GetCenterOffset();
    Vector2 myPos = Vector2(m_posX, m_posY) + GetCenterOffset();

    Vector2 direction = playerPos - myPos;
    direction.NormaliseInPlace();
    PrimeBullet(m_posX, m_posY, direction.x, direction.y, 100);
}

void Ent_Bullet_Light::Update(float dt)
{
    Ent_Bullet::Update(dt);

    //Reflection when bullet is already partially outside of boundary needs to be handled
    if ((m_posX < 0 || m_posX + k_centerOffset.x * 2 > SCR_WIDTH) && hasBeenDeflected && wallBounceCount < k_wallBounceLimit)
    {
        //To prevent from bound skipping
        if (m_posX < 0)
        {
            SetPosition(0, m_posY);
        }
        else if (m_posX > k_centerOffset.x * 2 > SCR_WIDTH)
        {
            SetPosition(m_posX > k_centerOffset.x * 2, m_posY);
        }
        ReflectOffWall();
    }
}

void Ent_Bullet_Light::PrimeBullet(float posX, float posY, float dirX, float dirY, float speed)
{
    Ent_Bullet::PrimeBullet(posX, posY, dirX, dirY, speed);
    hasBeenDeflected = false;
}

void Ent_Bullet_Light::DeflectBullet(int power, bool hyperDeflect)
{
    //first parameter is now irrelevant
    if (hasBeenDeflected) return;
    hasBeenDeflected = true;

    if (hyperDeflect)
    {
        m_damage = m_damage * LIGHTBULLET_HYPERDAMAGEMULTIPLIER;
        wallBounceCount -= LIGHTBULLET_HYPEREXTRAWALLBOUNCES;
    }
    /*m_velX = -m_velX;
    m_velY = -m_velY;*/

    Vector2 playerPos = playerPtr->GetPosition() + playerPtr->GetCenterOffset();
    Vector2 myPos = Vector2(m_posX, m_posY) + GetCenterOffset();

    Vector2 direction = myPos - playerPos;
    direction.NormaliseInPlace();
    PrimeBullet(m_posX, m_posY, direction.x, direction.y, (int) (power * (!hyperDeflect ? 1 : LIGHTBULLET_HYPERSPEEDMULTIPLIER)));
    hasBeenDeflected = true;
}

void Ent_Bullet_Light::ReflectOffWall()
{
    m_velX = -m_velX;
    wallBounceCount++;
}
