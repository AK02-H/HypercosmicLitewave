#pragma once
#include "Entity.h"
#include <HAPI_Types.h>
#include "Ent_Enemy.h"

//Rename to enemy data
struct Level_WaveEnemy
{
	int enemyTypeID{ 0 };
	Vector2 spawnPosition{ 0, 0 };
	Vector2 velocity{ 0, 0 };
	Vector2 acceleration{ 0, 0 };

	//other?
	Vector2 targetPosition{ 0, 0 };
};

struct Level_Wave
{
	std::string subWaveLabel = "w1";
	float timeOfOccurance{ 0 };
	std::vector<Level_WaveEnemy> enemiesOfWave;
};

struct Level_TimelineEvent
{
	std::string waveLabel = "Default";
	float retreatTime;
	std::vector<Level_Wave> waveCollection;
};



/*
LOADING LEVEL ENEMY WAVES FROM XML STRUCTURE:


rules:
all enemies (sans bosses) must retreat at some point
a WAVE ends when all enemies are taken out or retreat time is reached
a WAVE can have 'sub waves' that spawn at separate times during the waves


struct WAVE:
	enemyTypeID
	enemySpawnPosition
	enemyVelocity X and Y
	enemyAcceleration X and Y
	enemyMovementBehaviourID?



struct TIMELINEEVENT
	float timeofOccurance
	float retreatTime
	array of WAVEs



ENEMY WAVE SYSTEM:

	Loop through timeline events
	Once an event is activated, start a timer
	Once the timer reaches the event's retreat time, reset the timer and move onto the next event
		as a rule of thumb, all enemies must be hard-coded to retreat off screen before event's end time, less waves overlap

	During each wave, enemy manager should keep track of all enemies. If all enemies are dead then move onto the next wave.






Enemy types:
	Basic, move with velocity and acceleration, fire volley of bullets with timer
	Semi basic, fire bullets on timer, but moves to one position, stops, then retreats to a direction
	Kamikaze, moves onto certain point on screen with smoothing (either with lerp or physics), waits a while, then moves quickly towards player
	Delay, moves onto screen, probably from top, waits a while, fires big attack. either does again or leaves
	Sin/cos, moves according to math wave
	Math, moves according to curve
*/

class Ent_EnemyManager
	: public Entity
{
private:

	std::vector<Entity*>* all_entities;

	std::vector<Level_TimelineEvent> wholeAssLevel;
	Level_TimelineEvent* currentEvent;
	std::vector<Ent_Enemy*> monitorList; // theoretically never needs to be cleared?
	int currentMaxEnemies = 0;
	int deadEnemyCount = 0;


	int counter_wave{ 0 };
	int counter_SUBwave{ 0 };
	float timer_currentWave{ 0 };
	float timer_retreat{ 0 };
	bool stopCountingForThisWave = false;	//needed?
	bool stopCountingForThisSubWave = false;


public:


	Ent_EnemyManager(std::string levelFilePath);
	

	void Load() override final;

	void Update(float dt) override final;
	void Render(IGraphics& viz) override final {};	//does not need to be rendered
	void LateUpdate(float dt) override final {};

	void setAllEntities(std::vector<Entity*>* ptr)
	{
		all_entities = ptr;
	}

	void PrimeWave()
	{

	}

	void MoveOntoNextWave()
	{

	}

	void IncrementDeathCount() { deadEnemyCount++; }
};

