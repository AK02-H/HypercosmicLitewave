#pragma once
#include "Entity.h"
#include "Collider.h"
#include "Animator.h"

//currently a really bad bug where bullets can skip over enemies directly in front of it
//because of dt incrementing. Fix?
class Ent_Bullet :
    public Entity
{

private:

    Rectangle collisionBox;
    Collider collider{ &collisionBox };

protected:

    int m_damage = 5;

    int m_speed{ 1800 };
    float m_velX{ 0 }, m_velY{ 0 };

    Animator animator;

    virtual void BoundCheck()
    {
        //bound check to make inactive when out of screen
        if (m_posX > BULLETBOUND_MAX_X || m_posY > BULLETBOUND_MAX_Y || m_posX < BULLETBOUND_MIN_X || m_posY < BULLETBOUND_MIN_Y) isActive = false;
    }

public:

    bool animateBullet = false;

    Ent_Bullet(std::string gfxName)
    {
        m_gfxName = gfxName;
        animator = Animator(gfxName, 5);
        //animator.SetAnimation("test");
    }

    void Load() override final;

    void Update(float dt) override;
    void Render(IGraphics& viz) override;
    void LateUpdate(float dt) override final;

    void SetDamage(int newDamage) { m_damage = newDamage; }
    void SetPosition(float x, float y) override;
    void SetColliderRect(Rectangle& rect);
    
    Collider* GetCollider() { return &collider; }
    Animator* GetAnimator() { return &animator; }

    const int GetDamage() { return m_damage; }

    //Swap for vector2s later when the class is implemented,
    //don't forget to normalise direction as passed in
    virtual void PrimeBullet(float posX, float posY, float dirX, float dirY, float speed = 1800);
    
};

