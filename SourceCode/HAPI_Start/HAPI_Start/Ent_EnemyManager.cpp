#include "Ent_EnemyManager.h"

/*
* Enemy Types
* 0 = ENEMY_BASIC
* 1 = ENEMY_VERTICAL
* 2 = ENEMY_BURST
* 3 = ENEMY_KAMIKAZE
* 4 = ENEMY_MISSILE
*/


Ent_EnemyManager::Ent_EnemyManager(std::string levelFilePath)
{

	CHapiXML xml("Data/Levels/" + levelFilePath);
	//Currently no way to make sure this loads properly
	std::vector<CHapiXMLNode*> root = xml.GetAllNodesWithName("Root");
	std::vector<CHapiXMLNode*> allLevelWaves = xml.GetAllNodesWithName("Level_Wave");

	for (const auto& wave : allLevelWaves)
	{
		Level_TimelineEvent newTimelineEvent;

		//Set name and retreat time
		newTimelineEvent.waveLabel = wave->GetAttributes()[0].AsString();
		newTimelineEvent.retreatTime = wave->GetAttributes()[1].AsFloat();

		std::vector<CHapiXMLNode*> allSubWaves = wave->GetChildren();

		for (const auto& subWave : allSubWaves)
		{
			Level_Wave newWave;

			//Set name and occurance time
			newWave.subWaveLabel = subWave->GetAttributes()[0].AsString();
			newWave.timeOfOccurance = subWave->GetAttributes()[1].AsFloat();

			std::vector<CHapiXMLNode*> enemiesOfWave = subWave->GetChildren();

			for (const auto& enemy : enemiesOfWave)
			{

				//std::vector<CHapiXMLNode*> temp3 = o->GetChildren();

				Level_WaveEnemy newEnemy;

				newEnemy.enemyTypeID = enemy->GetAttributes()[0].AsInt();
				newEnemy.spawnPosition.x = enemy->GetAttributes()[1].AsInt();
				newEnemy.spawnPosition.y = enemy->GetAttributes()[2].AsInt();
				newEnemy.velocity.x = enemy->GetAttributes()[3].AsFloat();
				newEnemy.velocity.y = enemy->GetAttributes()[4].AsFloat();
				newEnemy.acceleration.x = enemy->GetAttributes()[5].AsFloat();
				newEnemy.acceleration.y = enemy->GetAttributes()[6].AsFloat();

				newWave.enemiesOfWave.push_back(newEnemy);
			}

			newTimelineEvent.waveCollection.push_back(newWave);

		}

		wholeAssLevel.push_back(newTimelineEvent);
	}


	//primes first wave, breaks if there is no waves at all but there should always be one in the file anyway
	//set max enemies (move to a function when wave moves on)
	currentMaxEnemies = 0;

	for (auto h : wholeAssLevel[counter_wave].waveCollection)
	{
		for (auto h2 : h.enemiesOfWave)
		{
			currentMaxEnemies++;
		}
	}
}

void Ent_EnemyManager::Load()
{
}

void Ent_EnemyManager::Update(float dt)
{

	if (!stopCountingForThisSubWave)
	{

		timer_currentWave += dt;

		if (timer_currentWave > wholeAssLevel[counter_wave].waveCollection[counter_SUBwave].timeOfOccurance)
		{
			//spawn this subwave
			//increment subwave counter
			//add all enemies of this wave to a monitor list, that will be checked (doesn't need to be every frame?), and will proceed the wave if all are destroyed
				//this monitor list grows as the wave proceeds and the subwaves come

			if (counter_wave == 1)
			{
				int dummy = 9;

			}

			//For each enemy of this subwave
			for (const Level_WaveEnemy& enemyData : wholeAssLevel[counter_wave].waveCollection[counter_SUBwave].enemiesOfWave)
			{
				//create and spawn enemy with this data

				//enemies should be destroyed after a certain amount of time, even after leaving the screen. just as a safety measure

				//use switch statement to pick which enemy		//this comment was useful

				Ent_Enemy* newEnemy{ nullptr };
				float spawnAttackDelay = 0;

				switch (enemyData.enemyTypeID)
				{
				case 0:	//Basic
				{
					newEnemy = static_cast<Ent_Enemy*>(RetrieveEntity(all_entities, ENEMY_BASIC_RANGE, true));
					spawnAttackDelay = NME_BASIC_INITIALFIREDELAY;
					break;
				}
				case 1:	//Vertical
				{
					newEnemy = static_cast<Ent_Enemy*>(RetrieveEntity(all_entities, ENEMY_VERTICAL_RANGE, true));
					spawnAttackDelay = NME_VERTICAL_INITIALFIREDELAY;
					break;
				}
				case 2:	//Burst
				{
					newEnemy = static_cast<Ent_Enemy*>(RetrieveEntity(all_entities, ENEMY_BURST_RANGE, true));
					spawnAttackDelay = NME_BURST_INITIALFIREDELAY;
					break;
				}
				case 3:	//Kamikaze
				{
					newEnemy = static_cast<Ent_Enemy*>(RetrieveEntity(all_entities, ENEMY_KAMIKAZE_RANGE, true));
					spawnAttackDelay = NME_KAMIKAZE_INITIALFIREDELAY;
					break;
				}
				case 4:	//Missile
				{
					newEnemy = static_cast<Ent_Enemy*>(RetrieveEntity(all_entities, ENEMY_MISSILE_RANGE, true));
					spawnAttackDelay = NME_MISSILE_INITIALFIREDELAY;
					break;
				}
				default: //Any
				{
					newEnemy = static_cast<Ent_Enemy*>(RetrieveEntity(all_entities, ENEMY_ALL_RANGE, true));
					break;
				}
				}

				//in enemy update add a funtion that deactivates itself after passing a boundary?
				//Have this boundary as a macro
				assert(newEnemy != nullptr && "WARNING: ENEMY WAS NULLPTR WHEN TRYING TO SPAWN");
				newEnemy->SetupEnemy(enemyData.spawnPosition, enemyData.velocity, enemyData.acceleration, spawnAttackDelay);	//this should also set its position
				newEnemy->SetActive(true);
				//monitorList


			}

			//do limit check

			if (counter_SUBwave + 1 > wholeAssLevel[counter_wave].waveCollection.size() - 1)
			{
				stopCountingForThisSubWave = true;
			}
			else
			{
				counter_SUBwave++;
			}

			timer_currentWave = 0;
		}

	}

	timer_retreat += dt;

	//std::cout << "CURRENT DEAD ENEMIES: " << deadEnemyCount << std::endl;

	//It must be guaranteed that all enemies are either dead or
	//retreated safely to start the next wave
	if (timer_retreat > wholeAssLevel[counter_wave].retreatTime || deadEnemyCount >= currentMaxEnemies)
	{
		if (deadEnemyCount >= currentMaxEnemies)
		{
			std::cout << "CURRENT WAVE ELIMINATED, SKIPPING TO NEXT WAVE" << std::endl;

			
		}


		if (counter_wave == wholeAssLevel.size() - 1)
		{
			std::cout << "REACHED END OF LEVEL" << std::endl;
			return;
		}

		//move onto next timeline event

		timer_retreat = 0;
		timer_currentWave = 0;
		deadEnemyCount = 0;

		stopCountingForThisSubWave = false;

		counter_SUBwave = 0;
		counter_wave++;

		//set max enemies (move to a function when wave moves on)
		currentMaxEnemies = 0;

		for (auto h : wholeAssLevel[counter_wave].waveCollection)
		{
			for (auto h2 : h.enemiesOfWave)
			{
				currentMaxEnemies++;
			}
		}
	}
}


