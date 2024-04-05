#pragma once
#include "Entity.h"
#include "IGraphics.h"
#include "InputHandler.h"
#include "Collider.h"
#include "Ent_HUDManager.h"
#include "Ent_Bullet.h"
#include "Ent_PlayerSaber.h"
#include "Ent_Enemy.h"
#include "Ent_AudioManager.h"

class Ent_Player :
   public Entity
{
private:

    IGraphics* vizPtr = nullptr; //for setting animations
    std::vector<Entity*>* all_entities{ nullptr };
    Ent_AudioManager* audioManager{ nullptr };
    Ent_HUDManager* hudManager;


    int m_lifeCount = PLAYER_START_LIVES;
    
    const Vector2 k_centerOffset{ 62.5f, 51.0f };

    int m_speed{ 500 };

    Vector2 velocity;

    //Have this here or define in world and only have a pointer? since input should be updating all the time
    //regardless of whether the player updates. Keep here for now to test
    InputHandler* inputter{ nullptr };
    Rectangle collisionBox{ 0, m_posX + dbgSz, 0, m_posY + dbgSz }; //change initial position
    Collider collider{ &collisionBox };
    Animator animator;

    int permanentRectWidth = 0;
    int permanentRectHeight = 0;

    bool canFire = true;
    const float k_fireInterval{ 0.05f };
    float timer_fireInterval{ 0 };

    bool isSecondPlayer = false;

    Ent_PlayerSaber* bulletSaber;

    const float k_flashIntervalRegular = 0.08f;
    const float k_flashIntervalHyper = 0.022f;
    float timer_chargeFlash = 0;
    bool isChargeFlash = false;

    const float k_maxRecoveryTime = 3;
    const float k_recoveryFlashTime = 0.06f;
    float timer_recoveryTime = 0;
    float timer_recoveryFlashInterval = 0;
    bool recoveryState = false;
    bool isRecoveryFlash = false;

    bool tempDeath = false;
    const float k_respawnTime = 1.5f;
    float timer_respawn = 0;;

    bool* deathMonitor{ nullptr };

public:


    Ent_Player(std::string gfxName, bool player2 = false)
    {
        m_gfxName = gfxName;
        isSecondPlayer = player2;
        animator = Animator(gfxName, FR_PLAYER);
        //GetAnimator()->SetAnimation(*vizPtr, "Neutral");
    }

    void InitInputHandler(InputHandler* newInput)
    {
        inputter = newInput;
    }

    void Load() override final;
    void Update(float dt) override final;
    void Render(IGraphics& viz) override;
    void LateUpdate(float dt) override final;


    //return pointer instead?
    Collider GetCollider() { return collider; }

    Animator* GetAnimator() { return &animator; }

    Vector2 GetCenterOffset() const { return k_centerOffset; }

    void OffsetCollider(int x, int y) { collisionBox.Translate(x, y); }

    void SetColliderRect(Rectangle& rect)
    {
        collisionBox.left = rect.left;
        collisionBox.right = rect.right;
        collisionBox.top = rect.top;
        collisionBox.bottom = rect.bottom;

        permanentRectWidth = rect.Width();
        permanentRectHeight = rect.Height();
    }

    void SetBulletSaber(Ent_PlayerSaber* saber) { bulletSaber = saber; }

    void SetGraphicSystemPtr(IGraphics* ptr) { vizPtr = ptr; }

    void SetAudioManager(Ent_AudioManager* ptr) { audioManager = ptr; }

    void SetHUDManager(Ent_HUDManager* hudPtr)
    { 
        hudManager = hudPtr;

        if (!isSecondPlayer)
        {
            hudManager->SetLifePtr_1(&m_lifeCount);
        }
        else
        {
            hudManager->SetLifePtr_2(&m_lifeCount);
        }
    }

    void SetEntityList(std::vector<Entity*>* entitiesPtr) { all_entities = entitiesPtr; }

    void SetDeathMonitorPointer(bool* point) { deathMonitor = point; }

    //Not needed anymore
    void SetIsSecondPlayer(bool is) { isSecondPlayer = is; }

    void CollisionCheck(std::vector<Entity*>* listOfEntities);
    
    void SetPosition(float x, float y) override;
    
    void FireBullet();
    
    void Die();

    void Respawn();
    
    
};

