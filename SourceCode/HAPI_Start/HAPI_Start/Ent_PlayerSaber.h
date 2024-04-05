#pragma once
#include "Entity.h"
#include "IGraphics.h"
#include "Animator.h"
#include "Rectangle.h"
#include "Collider.h"
#include "Ent_AudioManager.h"
//#include "Ent_Enemy.h"
#include "Ent_Bullet_Light.h"

//class Ent_Bullet_Light; //ask keith why these prototypes aren't working

class InputHandler;

class Ent_PlayerSaber :
    public Entity
{

private:

    IGraphics* vizPtr = nullptr; //for setting animations
    Ent_AudioManager* audioManager{ nullptr };
    InputHandler* rumbler = nullptr;   //ONLY USE FOR RUMBLE CALLS

    const int k_animationFrameRate = 30;
    const Vector2 offsetFromPlayer{ -42, -49 };
    const Vector2 offsetFromPlayer_hyper{ -77, -85 };

    const float k_hitstopSlowRate = 0.15f;
    const float k_hitstopDuration = 0.2f;
    float timer_hitstop{ 0 };
    bool hitstopActive = false;

    const int k_damageValue = SABER_BASEDAMAGE;
    const int k_hyperDamageValue = SABER_BASEDAMAGE;
    const int k_deflectPower = 1000;
    const int k_timeUntilHyperCharged = 1.5f;

    bool isCharging = false;
    float chargeTimer = 0;
    bool hyperSlashState = false;
    bool initialAnimationEnded = false;

    bool stopCheckingCollisions = false;

    bool hasPlayedChargeSound = false;

    Animator animator;  //investigate why is there access to this class without the #include

    //Collider error, collisions still happen when a rect's size is 0

    //Size of array should always be length of slash animation, find a way to set this
    Rectangle animatedHitboxesA[10] =
    {
        {3, 75, 83, 102},
        {43,81,8,54 },
        //{9,51,193,89},    add this frame back in?
        {116,199,93,130},
        {116,199,93,130},
        {110,193,55,104},
        {128,193,58,101},
        {131,200,77,101},
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100}

    };
    Rectangle animatedHitboxesB[10] =
    {
        {100,100,100,100},
        {43,95,55,100 },
        //{25,34,176,50 },
        {8,193,51,92 },
        {110,193,51,92 },
        {156,183,27,54 },
        {125,175,32,57 },
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100},

    };
    Rectangle animatedHitboxesC[10] =
    {
        {100,100,100,100},
        {21,42,33,101},
        //{36,16,153,33},
        {32,171,22,50},
        {32,171,22,50},
        {133,155,10,54},
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100},
    };

    Rectangle animatedHitboxesD[10] =
    {
        {100,100,100,100},
        {5,20,62,119},
        //{60,1,142,14},
        {59,139,2,21},
        {59,139,2,21},
        {101,132,0,54},
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100},
        {100,100,100,100},
    };


    Rectangle hyperAnimatedHitboxesA[18] =
    {
        {0,0,0,0},
        {216,240,205,225},
        {48,239,211,258},
        {99,157,3,175},
        {94,271,88,159},
        {94,271,88,159},
        {177,261,168,198},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137}
    };
    Rectangle hyperAnimatedHitboxesB[18] =
    {
        {0,0,0,0},
        {196,255,179,204},
        {64,109,166,210},
        {34,98,22,78},
        {39,247,31,87},
        {117,247,31,87},
        {116,271,118,167},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137}
    };

    Rectangle hyperAnimatedHitboxesC[18] =
    {
        {0,0,0,0},
        {170,265,146,178},
        {110,253,160,210},
        {12,98,79,245},
        {53,209,3,30},
        {117,209,3,30},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        /*{137,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},*/
    };

    Rectangle hyperAnimatedHitboxesD[18] =
    {
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137},
        {137,137,137,137}
    };

    Rectangle currentHboxA;
    Rectangle currentHboxB;
    Rectangle currentHboxC;
    Rectangle currentHboxD;

    Collider hitboxA{ &currentHboxA };
    Collider hitboxB{ &currentHboxB };
    Collider hitboxC{ &currentHboxC };
    Collider hitboxD{ &currentHboxD };

    void BeginHitstop();

public:

    bool prep = true;
    bool GetIsCharging() { return isCharging; }
    bool GetIsAtHyperTime() { return chargeTimer > k_timeUntilHyperCharged; }

    Ent_PlayerSaber(std::string gfxName)
    {
        m_gfxName = gfxName;
        animator = Animator(gfxName, k_animationFrameRate);

        //animator.SetAnimationFrame(0);
        currentHboxA = animatedHitboxesA[animator.frame];
        currentHboxB = animatedHitboxesB[animator.frame];
        currentHboxC = animatedHitboxesC[animator.frame];
        currentHboxD = animatedHitboxesD[animator.frame];
    }

    void Load() override final {};

    void Update(float dt) override final;
    void Render(IGraphics& viz) override final; //consider making these functions accessible to a friend world class
    void LateUpdate(float dt) override final;

    void SetGraphicSystemPtr(IGraphics* ptr)
    {
        vizPtr = ptr;
    }

    Animator* GetAnimator()
    {
        return &animator;
    }

    const int GetDamage() const
    {
        if (!hyperSlashState)
        {
            return k_damageValue;
        }
        else
        {
            return SABER_HYPERDAMAGE;
        }
    }

    void SetAudioManager(Ent_AudioManager* ptr) { audioManager = ptr; };
    
    const std::vector<Collider*> GetHitboxes();

    //just override set position?
   void UpdatePosition(float posX, float posY);

    void Slash(InputHandler* rumblerPtr = nullptr);
    void StartCharging();
    void ReleaseCharge(InputHandler* rumbler = nullptr);
    void ResetSaber();
    void CollisionCheck(std::vector<Entity*>* listOfEntities);
    

};

