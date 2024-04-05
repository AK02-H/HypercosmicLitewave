#pragma once
#include "Ent_Bullet.h"

class Ent_Player;

class Ent_Bullet_Light :
    public Ent_Bullet
{

private:

    bool hasBeenDeflected = false;
    int wallBounceCount = 0;
    const int k_wallBounceLimit = 3;
    //temp to fire in direction of player
    Ent_Player* playerPtr{ nullptr };

    //hardcoded center point of bullets
    const Vector2 k_centerOffset{ 19.5f, 19.5f };


public:

    //void 

    Ent_Bullet_Light(std::string gfxName) : Ent_Bullet(gfxName) //inherited constructor smells
    {
        m_gfxName = gfxName;
        animator = Animator(gfxName, FR_BULLET_LIGHT);
        //animator.SetAnimation("test");
    }
    
    void Update(float dt) override;

    void Render(IGraphics& viz) override;
    
    bool GetHasBeenDeflected() { return hasBeenDeflected; };
    void SetPlayerPtrAndDirection(Ent_Player* player);
    Vector2 GetCenterOffset() const { return k_centerOffset; }

    void PrimeBullet(float posX, float posY, float dirX, float dirY, float speed = 1800) override;
    void DeflectBullet(int power = 300, bool hyperDeflect = false);
    
    void ReflectOffWall();
   
};

