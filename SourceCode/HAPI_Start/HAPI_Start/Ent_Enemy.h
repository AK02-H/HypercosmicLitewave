#pragma once
#include "Entity.h"
#include "NGLOBALS.h"
#include "Collider.h"
#include "Ent_Bullet.h"
#include "Ent_Bullet_Light.h"
#include "Ent_Explosion.h"
#include "Ent_AudioManager.h"

#include <limits>

class Ent_Player;
class Ent_PlayerSaber;	//prototype for casting
class Ent_Explosion;
class Ent_HUDManager;
class Ent_EnemyManager; //only for recording dead enemies
class Ent_AudioManager;

class Ent_Enemy :
    public Entity
{
private:

	int m_maxHp{ 1 };
	int m_hp{ 150 };
	
	int* scorePtr{ nullptr };

	Rectangle collisionBox{ 0, m_posX + dbgSz, 0, m_posY + dbgSz }; //change initial position
	Collider collider{ &collisionBox };

	Vector2 hboxOffset{ 0, 0 };

	bool isMultiplayerMode = false;

	Ent_HUDManager* hudManager;


	bool declaredDead = false;

	bool saberInvulnerability = false;
	const float k_damageInvulnerabilityTime = 0.3f;
	float timer_damageInvulnerabilityTime = 0;
	bool isDamageFlashing{ false };
	const float k_damageFlashTime{ 0.09f };
	float timer_damageFlashTime{ 0 };


	

	int r = 50;
	int maxBullets = 3000;
	//use doubles or floats?
	double counterTime1 = 100;
	double counterWave1 = 0;
	double bulletTime1 = 0.1;
	size_t numBullet1 = 0;
	bool reversePattern;
	//std::vector<Ent_Bullet*> patternBullets;

protected:

	std::vector<Entity*>* all_entities{ nullptr };	//use this and global indexes to get references to players
	IGraphics* vizPtr = nullptr; //for setting animations
	Ent_AudioManager* audioManager{ nullptr };

	std::vector<Ent_Player*> all_players;

	int m_scoreValue = 250;	//make not const later

	Ent_Player* playerPtr{ nullptr };

	Animator animator;

	//Behaviour properties
	Vector2 velocity{ 0, 200 };
	Vector2 acceleration{ 0, -105 };


	float fireInterval{ 0.8f };
	float timer_fireInterval{ 0 };

	Vector2 fireOffset{ 0, 0 };


	virtual void FireBullet();
	virtual void HandleBullets();
	
	//Use this to despawn enemies after a while
	//Fairly necessary considering that acceleration isn't
	//dampened meaning they'll start going into infinite values if they aren't stopped
	bool IsInBounds();
	

public:

	Ent_Enemy(std::string gfxName)
	{
		m_gfxName = gfxName;
		animator = Animator(gfxName, 5);

	}

	void Load() override {};
	void Update(float dt) override;
	void Render(IGraphics& viz) override;
	void LateUpdate(float dt) override;

	void SetPosition(float x, float y) override;
	
	void SetInterval(float newInterval) { fireInterval = newInterval; }

	void SetMaxHP(float newMaxHP) { m_maxHp = newMaxHP; m_hp = m_maxHp; }

	void SetScoreValue(int newScore) { m_scoreValue = newScore; }

	void SetHboxOffset(Vector2 offset) { hboxOffset = offset; }

	void SetInitialFireDelay(float delay) { timer_fireInterval = 0 - delay; }

	void SetColliderRect(Rectangle& rect) { collisionBox = rect; }

	void SetGraphicSystemPtr(IGraphics* ptr) { vizPtr = ptr; }

	void SetAudioManager(Ent_AudioManager* ptr) { audioManager = ptr; }

	void SetFireOffset(Vector2 newOffset) { fireOffset = newOffset; }

	void SetEntityList(std::vector<Entity*>* entitiesPtr) { all_entities = entitiesPtr; }

	void SetPlayerPtr(Ent_Player* ptr) { playerPtr = ptr; }

	void SetScorePtr(int* ptr) { scorePtr = ptr; }

	void SetIsMultiplayerMode(bool use) { isMultiplayerMode = use; }

	void SetHUDManager(Ent_HUDManager* hudPtr) { hudManager = hudPtr; }

	Animator* GetAnimator() { return &animator; }

	Collider* GetCollider() {return &collider; }

	virtual void SetupEnemy(Vector2 position, Vector2 velocity, Vector2 acceleration, float fireDelay = 0);

	void SetupPlayerList();

	//Keep public in case i need to use it for some complex attack patterns
	Ent_Player* FindClosestPlayer();
	
	void TakeDamage(int damage, bool wasFromLight = false);
	
	virtual void Die();
	
	virtual void SpawnDeathExplosion();
	
	virtual void CollisionCheck(std::vector<Entity*>* listOfEntities);
	
};

//POSSIBLE PROPERTIES:
/*
Stop at 0 velocity
Follow sine wave
Follow cosine wave

*/