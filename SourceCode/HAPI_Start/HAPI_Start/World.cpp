#include "World.h"
#include "UIElement.h"
#include "UI_Button.h"
#include "Ent_AudioManager.h"
#include "Ent_Player.h"	//should this be in h or cpp file
#include "Ent_Bullet.h"
#include "Ent_Bullet_Light.h"
#include "Ent_Background.h"
#include "Ent_EnemyManager.h"
#include "Ent_PlayerSaber.h"
#include "ENEMYMASTER.h"



void World::LoadLevel()
{

}

//No longer needed
void World::Update()
{
	
}

eGameReturnQ World::Run()
{
	

	int currentScore = 0;
	bool monitorP1Death = false;
	bool monitorP2Death = false;
	bool GAMEOVER = false;


	//DON'T FORGET TO PRIME RANDOM SEED?
	//don't forget to prime random seed at start
	//Do in main?
	srand(time(NULL));


	SauceyGlobals::GetInstance().debugColliders = collidersOn;
	SauceyGlobals::GetInstance().stereoAudio = stereoOn;
	
	float difficultyScaler = 1;
	float speedScaler = 1;
	//Value that is usually applied to things that are time-based, makes things go faster
	switch (gameDifficulty)
	{
	case eDifficulty::eEasy:
		difficultyScaler = 1.5f;
		SauceyGlobals::GetInstance().maxLives = 7;
		break;
	case eDifficulty::eNormal:
		difficultyScaler = 1.0f;
		SauceyGlobals::GetInstance().maxLives = 5;
		break;
	case eDifficulty::eHard:
		difficultyScaler = 0.7f;
		SauceyGlobals::GetInstance().maxLives = 3;
		break;
	default:
		break;
	}

	SauceyGlobals::GetInstance().difficultyScale = difficultyScaler;

	switch (gameSpeed)
	{
	case eGameSpeed::e25:
		speedScaler = 0.25f;
		break;
	case eGameSpeed::e50:
		speedScaler = 0.5f;
		break;
	case eGameSpeed::e100:
		speedScaler = 1;
		break;
	case eGameSpeed::e150:
		speedScaler = 1.5f;
		break;
	case eGameSpeed::e200:
		speedScaler = 2.0f;
		break;
	default:
		break;
	}

	Chron::GetInstance().timeBASE = speedScaler;
	Chron::GetInstance().timescale = speedScaler;

	int width{ SCR_WIDTH };
	int height{ SCR_HEIGHT };

	HAPI.SetShowFPS(true);

	/*if (!HAPI.Initialise(width, height))
		return;*/

	BYTE* screen{ HAPI.GetScreenPointer() };
	visual = LoadGraphicSystem(EGraphicsSystem::eHAPI);

	assert(visual != nullptr);

	visual->Init(width, height);

	#pragma region Create sprites

	//Loading graphics
	visual->CreateSprite(NULLTEX_STR, "Graphics/texMissing.png");
	visual->CreateSprite("player", "Graphics/playerCharacterSheet_extra.png", true, "Graphics/playerSprite_extra_anim.xml");
	visual->CreateSprite("player2", "Graphics/playerCharacterSheet_extra_2.png", true, "Graphics/playerSprite_extra_anim.xml");
	visual->CreateSprite("saber", "Graphics/saberSlice_stitched.png", true, "Graphics/saberPlaceholder_anim.xml");
	visual->CreateSprite("saber2", "Graphics/saberSlice_stitched_B.png", true, "Graphics/saberPlaceholder_anim.xml");

	visual->CreateSprite("enemyPop", "Graphics/enemy_popcorn.png", true, "Graphics/enemy_popcorn_anim.xml");
	visual->CreateSprite("enemyVert", "Graphics/enemy_4.png", true, "Graphics/enemy_4_anim.xml");
	visual->CreateSprite("enemyBurst", "Graphics/enemy_burster.png", true, "Graphics/enemy_burster_anim .xml");
	visual->CreateSprite("enemyKamikaze", "Graphics/enemy_kamikaze.png", true, "Graphics/enemy_kamikaze_anim.xml");
	//accidentally used the wrong sprite for this at first but it looked really cool i think
	visual->CreateSprite("enemyMissile", "Graphics/enemy_missile_2.png", true, "Graphics/enemy_missile_anim.xml");

	visual->CreateSprite("bullet_player_A", "Graphics/playerBullet_A.png");
	visual->CreateSprite("bullet_player_B", "Graphics/playerBullet_B2.png");
	visual->CreateSprite("bullet_enemy_A", "Graphics/enemyBullet_A.png");
	visual->CreateSprite("bullet_light", "Graphics/bullet_lightwave.png", true, "Graphics/bullet_light_anim.xml");

	visual->CreateSprite("explosion_1", "Graphics/FX/expl_1.png", true, "Graphics/FX/explosion_generic_anim.xml");
	visual->CreateSprite("explosion_2", "Graphics/FX/expl_2.png", true, "Graphics/FX/explosion_generic_anim.xml");
	visual->CreateSprite("explosion_3", "Graphics/FX/expl_3.png", true, "Graphics/FX/explosion_generic_anim.xml");
	visual->CreateSprite("explosion_4", "Graphics/FX/expl_4.png", true, "Graphics/FX/explosion_generic_anim.xml");
	visual->CreateSprite("explosion_5", "Graphics/FX/expl_5.png", true, "Graphics/FX/explosion_generic_anim.xml");
	visual->CreateSprite("explosion_6", "Graphics/FX/expl_6.png", true, "Graphics/FX/explosion_generic_anim.xml");
	visual->CreateSprite("explode_player", "Graphics/FX/expl_player.png", true, "Graphics/FX/explosion_player.xml");

	//visual->CreateSprite("bg", "testBackground.png");
	visual->CreateSprite("bg1", "Graphics/placeholderBg_1.png");
	visual->CreateSprite("bg2", "Graphics/placeholderBg_2.png");

	//UI elements for gameover screen
	visual->CreateSprite("gameOverBackground", "Graphics/ui_gameOverBg.png");
	visual->CreateSprite("gameOverButtons", "Graphics/ui_gameOverButtons.png", true, "Graphics/UIGameOverButtons_list.xml");

	#pragma endregion

	#pragma region Entity creation

	//Managers should be made first because there'll only be one of each, so it'll look better in NGLOBALS
	#pragma region create managers

	//Created first but the only entity to not be pushed in order
	//either push this entity last or render separately after everything else in a separate loop
	Ent_HUDManager* HUD = new Ent_HUDManager();
	HUD->SetActive(true);
	HUD->SetScorePtr(&currentScore);
	HUD->SetIsMultiplayerGame(m_isLocalMultiplayerGame);

	//Create level manager
	Ent_EnemyManager* levelLoader = new Ent_EnemyManager("testLevel.xml");
	levelLoader->SetActive(true);
	levelLoader->setAllEntities(&m_entities);
	m_entities.push_back(levelLoader);

	float finalSoundVolume = (float)m_sfxVolume / (float)100;
	float finalMusicVolume = (float)m_mxVolume / (float)100;
	Ent_AudioManager* audioManager = new Ent_AudioManager(finalSoundVolume, finalMusicVolume);
	audioManager->AddNewSound("playerfire.wav");				//ID 0
	audioManager->AddNewSound("playerslash.wav");				//ID 1
	audioManager->AddNewSound("playerslashHyper.wav");			//ID 2
	audioManager->AddNewSound("playerHyperChargeReady.wav");	//ID 3
	audioManager->AddNewSound("explosion1.wav");				//ID 4
	audioManager->AddNewSound("bulletDeflect.wav");				//ID 5
	audioManager->AddNewSound("enemyfire.wav");					//ID 6
	audioManager->AddNewSound("playerDie.wav");					//ID 7
	audioManager->AddNewSound("playerRespawn.wav");				//ID 8
	audioManager->AddNewSound("nuke.wav");						//ID 9
	audioManager->AddNewSound("nukeDie.wav");					//ID 10
	audioManager->AddNewSound("kamikazeSpawn.wav");				//ID 11
	audioManager->AddNewSound("kamikazeLaunch.wav");			//ID 12

	//Technically does not need to be pushed to m_entities, not
	//even need to be an entity itself, but am doing for consistency sake
	m_entities.push_back(audioManager);

	#pragma endregion

	//being made first so it's always rendered first, but later on when render order is implemented this isn't necessary
	#pragma region create background

	Ent_Background* bg = new Ent_Background();
	bg->SetActive(true);

	bg->AddNewLayer("bg1", { -20, 200 }, eScrollType::HorizontalAndVertical, eScrollDirection::HorizBACK_Vert_FOR);
	bg->AddNewLayer("bg2", { 400, 400 }, eScrollType::HorizontalAndVertical, eScrollDirection::HorizFOR_Vert_FOR);

	m_entities.push_back(bg);

	#pragma endregion


	//mention that it's a disadvantage that entities need to reference the viz system
	//to change animations at runtime

	#pragma region create player and saber entities


	//Does load need to be called on all entities?

	#pragma region first player

	Ent_Player* player_1 = new Ent_Player("player", false);
	player_1->Load();	//should player be loaded before input handler is initialized?
	player_1->SetGraphicSystemPtr(visual);
	player_1->SetEntityList(&m_entities);
	player_1->SetAudioManager(audioManager);
	player_1->SetActive(true);
	player_1->GetAnimator()->SetAnimation(*visual, "Neutral");

	InputHandler* playerInput_1 = nullptr;
	
	//FIX CONTROLLERS BEFORE HANDING IN
	if (controlSettingsID == 0)	//P1 KEYBOARD P2 CONTROLLER
	{
		playerInput_1 = new InputHandler(ControllerType::typeKeyboard);
		//playerInput_1->SetControllerID(1);

	}
	else if (controlSettingsID == 1) //P1 CONTROLLER P2 KEYBOARD
	{
		playerInput_1 = new InputHandler(ControllerType::typeController);
		//playerInput_1->SetControllerID(0);

	}
	else if (controlSettingsID == 2) //P1 CONTROLLER P2 CONTROLLER
	{
		playerInput_1 = new InputHandler(ControllerType::typeController);
		playerInput_1->SetControllerID(0);
	}
	else
	{
		playerInput_1 = new InputHandler(ControllerType::typeKeyboard);
	}

	player_1->InitInputHandler(playerInput_1);
	m_allInputHandlers.push_back(playerInput_1);

	player_1->SetPosition(PLAYER1_SPAWNPOS);

	//Setup collider
	Rectangle rect_player_1 = Rectangle(0, 23, 0, 23);
	player_1->SetColliderRect(rect_player_1);
	player_1->OffsetCollider(48, 21);	//hardcode these values somewhere

	//shouldn't matter if components are assigned before player is added to m_entities, bc pointers
	//consider pushing entities in clumps to be able to monitor them easier

	//Create Saber
	Ent_PlayerSaber* saber_1 = new Ent_PlayerSaber("saber");
	saber_1->SetActive(false);
	saber_1->GetAnimator()->SetAnimation(*visual, "Slash");
	saber_1->GetAnimator()->shouldLoop = false;
	saber_1->SetGraphicSystemPtr(visual);
	saber_1->SetAudioManager(audioManager);
	player_1->SetBulletSaber(saber_1);


	#pragma endregion

	#pragma region second player

	//Still create even if 2player is off, just in case I decide to add joining mid game

	Ent_Player* player_2 = new Ent_Player("player2", true);

	player_2->Load();	//should player be loaded before input handler is initialized?
	player_2->SetGraphicSystemPtr(visual);
	player_2->SetEntityList(&m_entities);
	player_2->SetAudioManager(audioManager);
	player_2->SetActive(true);
	player_2->GetAnimator()->SetAnimation(*visual, "Neutral");

	//Do something about this
	InputHandler* playerInput_2 = nullptr;

	if (controlSettingsID == 0) //P1 KEYBOARD P2 CONTROLLER
	{
		playerInput_2 = new InputHandler(ControllerType::typeController);
		//playerInput_2->SetControllerID(0);

	}
	else if (controlSettingsID == 1) //P1 CONTROLLER P2 KEYBOARD
	{
		playerInput_2 = new InputHandler(ControllerType::typeKeyboard);
		//playerInput_2->SetControllerID(1);

	}
	else if (controlSettingsID == 2) //P1 CONTROLLER P2 CONTROLLER
	{
		playerInput_2 = new InputHandler(ControllerType::typeController);
		playerInput_2->SetControllerID(1);
	}
	else //emergency
	{
		playerInput_2 = new InputHandler(ControllerType::typeKeyboard);
	}

	player_2->InitInputHandler(playerInput_2);
	m_allInputHandlers.push_back(playerInput_2);

	player_2->SetPosition(PLAYER2_SPAWNPOS);

	//Setup collider
	Rectangle rect_player_2 = Rectangle(0, 23, 0, 23);
	player_2->SetColliderRect(rect_player_2);
	player_2->OffsetCollider(48, 21);	//hardcode these values somewhere

	//shouldn't matter if components are assigned before player is added to m_entities, bc pointers
	//consider pushing entities in clumps to be able to monitor them easier

	//Create Saber
	Ent_PlayerSaber* saber_2 = new Ent_PlayerSaber("saber2");
	saber_2->SetActive(false);
	saber_2->GetAnimator()->SetAnimation(*visual, "Slash");
	saber_2->GetAnimator()->shouldLoop = false;
	saber_2->SetGraphicSystemPtr(visual);
	saber_2->SetAudioManager(audioManager);
	player_2->SetBulletSaber(saber_2);

	#pragma endregion
	
	player_1->SetHUDManager(HUD);
	player_2->SetHUDManager(HUD);
	player_1->SetDeathMonitorPointer(&monitorP1Death);
	player_2->SetDeathMonitorPointer(&monitorP2Death);

	if (!m_isLocalMultiplayerGame)
	{
		saber_2->SetActive(false);
		player_2->SetActive(false);
	}

	//pushed in this order because of rendering layers
	//(also saber would preferably be above enemies)
	m_entities.push_back(saber_1);
	m_entities.push_back(saber_2);
	m_entities.push_back(player_1);
	m_entities.push_back(player_2);


	#pragma endregion

	#pragma region create bullets

	
	
	#pragma region Player Bullets

	//Won't work for animated sprites currently, must be hardcoded
	Rectangle rect_bullet_player_A = Rectangle(0, visual->GetSpriteWidth("bullet_player_A"), 0, visual->GetSpriteHeight("bullet_player_A"));

	for (size_t i = 0; i < BULLETSPLAYER_COUNT; i++)
	{
		Ent_Bullet* newBullet = new Ent_Bullet("bullet_player_A");

		newBullet->Load();
		newBullet->SetColliderRect(rect_bullet_player_A);

		m_entities.push_back(newBullet);

	}

	for (size_t i = 0; i < BULLETSPLAYER_B_COUNT; i++)
	{
		Ent_Bullet* newBullet = new Ent_Bullet("bullet_player_B");



		newBullet->Load();
		newBullet->SetColliderRect(rect_bullet_player_A);

		m_entities.push_back(newBullet);

	}

	#pragma endregion

	#pragma region Enemy Bullets

	Rectangle rect_bullet_enemy_A = Rectangle(0, 19, 0, 19);

	for (size_t i = 0; i < BULLETS_ENEMY_A_COUNT; i++)
	{
		Ent_Bullet* newBullet = new Ent_Bullet("bullet_enemy_A");	//find out what new keyword does again


		//newBullet->GetAnimator()->SetAnimation(*visual, "test");

		newBullet->Load();
		newBullet->SetColliderRect(rect_bullet_enemy_A);


		m_entities.push_back(newBullet);
		//m_allColliders.push_back(&newBullet->GetCollider());

	}

	#pragma endregion

	#pragma region Light Bullets

	Rectangle rect_bullet_light = Rectangle(0, 39, 0, 39);
	int activeCount = 0;
	for (size_t i = 0; i < BULLETS_LIGHT_COUNT; i++)
	{
		Ent_Bullet_Light* newBullet = new Ent_Bullet_Light("bullet_light");	//find out what new keyword does again


		//
		int x = rand() % SCR_WIDTH;
		int y = rand() % 300;

		newBullet->Load();
		newBullet->SetColliderRect(rect_bullet_light);

		newBullet->GetAnimator()->SetAnimation(*visual, "Idle");

		newBullet->SetPosition(x, y);

		newBullet->SetPlayerPtrAndDirection(player_1);

		if (i == activeCount)
		{
			newBullet->SetActive(true);
		}
		else
		{
			newBullet->SetActive(false);
		}
		newBullet->animateBullet = true;

		newBullet->SetDamage(LIGHTBULLET_BASEDAMAGE);

		m_entities.push_back(newBullet);
		//m_allColliders.push_back(&newBullet->GetCollider());


		activeCount++;
		if (activeCount > BULLETS_LIGHT_COUNT / 10)
		{
			activeCount = BULLETS_LIGHT_COUNT / 10;
		}
	}

	#pragma endregion


	#pragma endregion

	#pragma region create enemies

	#pragma region Enemy Basic

	Rectangle rect_enemy_A = Rectangle(HB_EN_BASIC);
	int ___ = 1;	//tried to see if it was possible to name a variable with just underscores lol
	bool flipperooni = false;

	for (size_t i = 0; i < ENEMY_BASIC_COUNT; i++)
	{
		Ent_Enemy_Basic* enemy = new Ent_Enemy_Basic("enemyPop");
		enemy->Load();	//needed?
		enemy->SetPosition(0, 0);
		enemy->SetFireOffset({ 18, 50 });
		enemy->SetEntityList(&m_entities);
		enemy->SetActive(false);
		enemy->SetColliderRect(rect_enemy_A);
		enemy->SetHboxOffset(HB_EN_BASIC_OFFSET);

		enemy->GetAnimator()->SetAnimation(*visual, "Idle");
		enemy->SetScorePtr(&currentScore);
		enemy->SetIsMultiplayerMode(m_isLocalMultiplayerGame);
		enemy->SetHUDManager(HUD);
		enemy->SetAudioManager(audioManager);
		enemy->SetupPlayerList();
		enemy->id = i;

		if (flipperooni) enemy->fireLightBullet;
		flipperooni = !flipperooni;

		enemy->SetMaxHP(NME_BASIC_MAXHP);
		enemy->SetInterval(NME_BASIC_FIREINTERVAL);
		enemy->SetInitialFireDelay(NME_BASIC_INITIALFIREDELAY);
		enemy->SetScoreValue(NME_BASIC_SCORE);

		m_entities.push_back(enemy);
	}

	#pragma endregion

	#pragma region Enemy Vertical

	Rectangle rect_enemy_B = Rectangle(HB_EN_VERTICAL);

	for (size_t i = 0; i < ENEMY_VERTICAL_COUNT; i++)
	{
		Ent_Enemy_Vertical* enemy = new Ent_Enemy_Vertical("enemyVert");
		enemy->Load();	//needed?
		enemy->SetPosition(0, 0);
		enemy->SetFireOffset({ 18, 50 });
		enemy->SetEntityList(&m_entities);
		enemy->SetActive(false);
		enemy->SetColliderRect(rect_enemy_B);
		enemy->SetHboxOffset(HB_EN_VERTICAL_OFFSET);

		enemy->GetAnimator()->SetAnimation(*visual, "Idle");
		enemy->SetScorePtr(&currentScore);
		enemy->SetIsMultiplayerMode(m_isLocalMultiplayerGame);
		enemy->SetHUDManager(HUD);
		enemy->SetAudioManager(audioManager);
		enemy->SetupPlayerList();
		enemy->id = i;

		enemy->SetMaxHP(NME_VERTICAL_MAXHP);
		enemy->SetInterval(NME_VERTICAL_FIREINTERVAL);
		enemy->SetScoreValue(NME_VERTICAL_SCORE);

		m_entities.push_back(enemy);
	}

	#pragma endregion

	#pragma region Enemy Burst

	Rectangle rect_enemy_C = Rectangle(HB_EN_BURST);

	for (size_t i = 0; i < ENEMY_BURST_COUNT; i++)
	{
		Ent_Enemy_Burst* enemy = new Ent_Enemy_Burst("enemyBurst");
		enemy->Load();	//needed?
		enemy->SetPosition(0, 0);
		enemy->SetFireOffset({ 18, 50 });
		enemy->SetEntityList(&m_entities);
		enemy->SetActive(false);
		enemy->SetColliderRect(rect_enemy_C);
		enemy->SetHboxOffset(HB_EN_BURST_OFFSET);

		enemy->GetAnimator()->SetAnimation(*visual, "Charge");
		enemy->GetAnimator()->shouldLoop = true;
		enemy->SetScorePtr(&currentScore);
		enemy->SetIsMultiplayerMode(m_isLocalMultiplayerGame);
		enemy->SetHUDManager(HUD);
		enemy->SetAudioManager(audioManager);
		enemy->SetupPlayerList();
		enemy->id = i;

		enemy->SetMaxHP(NME_BURST_MAXHP);
		//enemy->SetInterval(NME_VERTICAL_FIREINTERVAL);
		enemy->SetScoreValue(NME_BURST_SCORE);


		m_entities.push_back(enemy);
	}

	#pragma endregion

	#pragma region Enemy Kamikaze

	Rectangle rect_enemy_D = Rectangle(HB_EN_KAMIKAZE);

	for (size_t i = 0; i < ENEMY_KAMIKAZE_COUNT; i++)
	{
		Ent_Enemy_Kamikaze* enemy = new Ent_Enemy_Kamikaze("enemyKamikaze");
		enemy->Load();	//needed?
		enemy->SetPosition(0, 0);
		enemy->SetFireOffset({ 18, 50 });
		enemy->SetEntityList(&m_entities);
		enemy->SetActive(false);
		enemy->SetColliderRect(rect_enemy_D);
		enemy->SetHboxOffset(HB_EN_KAMIKAZE_OFFSET);
		
		enemy->SetScorePtr(&currentScore);
		enemy->SetIsMultiplayerMode(m_isLocalMultiplayerGame);
		enemy->SetHUDManager(HUD);
		enemy->SetGraphicSystemPtr(visual);
		enemy->SetAudioManager(audioManager);
		enemy->SetupPlayerList();
		enemy->id = i;

		enemy->GetAnimator()->shouldLoop = false;
		enemy->GetAnimator()->SetAnimation(*visual, "Enter");

		enemy->SetMaxHP(NME_KAMIKAZE_MAXHP);
		//enemy->SetInterval(NME_VERTICAL_FIREINTERVAL);
		enemy->SetScoreValue(NME_KAMIKAZE_SCORE);


		m_entities.push_back(enemy);
	}

	#pragma endregion

	//Note: it seems that the bug of colliders appearing offscreen is purely visual and exclusive to debug views,
	//does not affect the actual collision box of the enemy
	#pragma region Enemy Missile

	Rectangle rect_enemy_E = Rectangle(HB_EN_MISSILE);

	for (size_t i = 0; i < ENEMY_MISSILE_COUNT; i++)
	{
		Ent_Enemy_Missile* enemy = new Ent_Enemy_Missile("enemyMissile");
		enemy->Load();	//needed?
		enemy->SetPosition(0, 0);
		//enemy->SetFireOffset({ 18, 50 });
		enemy->SetEntityList(&m_entities);
		enemy->SetActive(false);
		enemy->SetColliderRect(rect_enemy_E);
		enemy->SetHboxOffset(HB_EN_MISSILE_OFFSET);

		enemy->SetScorePtr(&currentScore);
		enemy->SetIsMultiplayerMode(m_isLocalMultiplayerGame);
		enemy->SetHUDManager(HUD);
		enemy->SetGraphicSystemPtr(visual);
		enemy->SetAudioManager(audioManager);
		enemy->SetupPlayerList();
		enemy->id = i;

		enemy->GetAnimator()->shouldLoop = true;
		enemy->GetAnimator()->SetAnimation(*visual, "Idle");

		enemy->SetMaxHP(NME_MISSILE_MAXHP);
		enemy->SetInterval(NME_MISSILE_FIREINTERVAL);
		enemy->SetScoreValue(NME_MISSILE_SCORE);


		m_entities.push_back(enemy);
	}

	#pragma endregion

	#pragma endregion

	#pragma region create explosion FX

	std::string explosionSprites[6] =
	{
		"explosion_1",
		"explosion_2",
		"explosion_3",
		"explosion_4",
		"explosion_5",
		"explosion_6"
	};

	for (size_t i = 0; i < EXPLOSION_COUNT; i++)
	{
		Ent_Explosion* explosion = new Ent_Explosion(explosionSprites[rand() % 6], FR_EXPLOSION_1);
		explosion->Load();
		explosion->GetAnimator()->SetAnimation(*visual, "Explode");

		explosion->SetActive(false);
		m_entities.push_back(explosion);
	}

	//Create type B explosion

	for (size_t i = 0; i < EXPLOSION_TYPEB_COUNT; i++)
	{
		Ent_Explosion* explosion = new Ent_Explosion("explode_player", FR_EXPLOSION_2);
		explosion->Load();
		explosion->GetAnimator()->SetAnimation(*visual, "Explode");

		explosion->SetActive(false);
		m_entities.push_back(explosion);
	}

	#pragma endregion

	//Finally push hud to entity list
	m_entities.push_back(HUD);

	#pragma endregion

	#pragma region game over UI setup

	//create menus for HUD i.e game over and pause

	std::vector<UIElement*> gameOverUIElements;
	UIElement gmvr_background("gameOverBackground");
	gmvr_background.currentSpriteEffectDefault = SpriteEffect::eHalfAlpha;
	gameOverUIElements.push_back(&gmvr_background);

	std::vector<UI_Button*> gameOverUIButtons;

	/*Button queries for game over screen:
	* 0 = retry, start another recursive loop from the main menu function
	* 1 = return to menu, return to main menu and prompt it to reset which menu it's on
	* 2 = quit game
	*/

	UI_Button gmvr_button_retry("gameOverButtons");
	gmvr_button_retry.commandId = 1;
	gmvr_button_retry.SetBoundingRect(BUTTON_C_RECT);
	gmvr_button_retry.SetPosition(SCR_WIDTH / 2 - gmvr_button_retry.GetRect().Width() / 2, 400);
	gmvr_button_retry.GetAnimator()->SetAnimation(*visual, "Retry");
	gameOverUIElements.push_back(&gmvr_button_retry);
	gameOverUIButtons.push_back(&gmvr_button_retry);

	UI_Button gmvr_button_menu("gameOverButtons");
	gmvr_button_menu.commandId = 2;
	gmvr_button_menu.SetBoundingRect(BUTTON_C_RECT);
	gmvr_button_menu.SetPosition(SCR_WIDTH / 2 - gmvr_button_menu.GetRect().Width() / 2, 480);
	gmvr_button_menu.GetAnimator()->SetAnimation(*visual, "Menu");
	gameOverUIElements.push_back(&gmvr_button_menu);
	gameOverUIButtons.push_back(&gmvr_button_menu);

	UI_Button gmvr_button_quit("gameOverButtons");
	gmvr_button_quit.commandId = 3;
	gmvr_button_quit.SetBoundingRect(BUTTON_C_RECT);
	gmvr_button_quit.SetPosition(SCR_WIDTH / 2 - gmvr_button_quit.GetRect().Width() / 2, 560);
	gmvr_button_quit.GetAnimator()->SetAnimation(*visual, "Quit");
	gameOverUIElements.push_back(&gmvr_button_quit);
	gameOverUIButtons.push_back(&gmvr_button_quit);

	int currentButtonQuery = 0;

	//set mouse data for all buttons

	const HAPISPACE::HAPI_TMouseData& m_mouseData(HAPI.GetMouseData());

	for (UI_Button*& b : gameOverUIButtons)
	{
		b->SetMouseData(&m_mouseData);
		//b->SetAnimation(*visual, "Idle");
	}

	#pragma endregion


	float lastTick = (float) HAPI.GetTime();
	float dt = 0;

	double lastTickD = (double)HAPI.GetTime();
	float dtD = 0;	//?????? shouldnt this be a double


	while (HAPI.Update()) {

		dt = ((float) HAPI.GetTime() - lastTick) / 1000.0f;
		lastTick = (float) HAPI.GetTime();

		dtD = ((double)HAPI.GetTime() - lastTickD) / 1000.0f;
		lastTickD = (double)HAPI.GetTime();

		//modify dt to timescale
		dt *= TIMESCALE;
		dtD *= (double)TIMESCALE;

		/*dt *= g_timescale;
		dtD *= (double)g_timescale;*/

		//std::cout << "DELTA DOUBLE IS " << dtD << std::endl;
		//std::cout << "TIMESCALE IS " << g_timescale << std::endl;

		visual->ClearScreen(120, 120, 240);	 


		//HANDLE INPUT
		for (InputHandler* input : m_allInputHandlers)
		{
			input->Handle(dtD);
		}

		//mention I considered a serparate render order list
		//mention the purpose of late update
		//UPDATE AND RENDER ENTITIES
		if (!GAMEOVER)
		{
			for (Entity* ent : m_entities)
			{
				if (ent->isActive)
				{
					//Update all separately?
					ent->Update(dtD);
					ent->Render(*visual);
					ent->LateUpdate(dtD);
				}

			}
		}
		else //handle game over screen
		{
			for (Entity* ent : m_entities)
			{
				if (ent->isActive)
				{
					ent->Render(*visual);

				}

			}

			for (UIElement* ent : gameOverUIElements)
			{
				if (ent->isActive)
				{
					ent->Update(dt);
					ent->Render(*visual);
				}

			}


			for (UI_Button*& b : gameOverUIButtons)
			{
				if (b->isPressed)
				{
					currentButtonQuery = b->commandId;
					break;
				}
			}


			//This works by returning from the entire run function with an enum that
			//defines what the next step should be
			switch (currentButtonQuery)
			{
				case 0:	//Do nothing
				{
					break;
				}
				case 1:	//retry
				{
					std::cout << "RETRY" << std::endl;
					return eGameReturnQ::eRestart;
					break;
				}
				case 2: //menu
				{
					std::cout << "MENU" << std::endl;
					return eGameReturnQ::eMenu;
					break;
				}
				case 3: //quit application (return instead of doing it here to clean up memory)
				{
					std::cout << "QUIT" << std::endl;
					return eGameReturnQ::eQuitGame;
					break;
				}
				default:
				{
					break;
				}
			}
		}


		if ((monitorP1Death && !m_isLocalMultiplayerGame) || (monitorP1Death && monitorP2Death && m_isLocalMultiplayerGame))
		{
			std::cout << "GAME OVER" << std::endl;
			GAMEOVER = true;
			audioManager->StopMusic();
			//setup gameover screen
		}

		#pragma region Prompt entities to check collisions


		//Colliders

		//Collision 'matrix'
		/*
		* Note down which index ranges/single indexes to do collision checks in
		Player checks for collision with all enemy bullets (if within distance, i.e use distance measurements to prevent unecessary calculations)
		All enemies check for collisions with

		player = 0
		player bullets = 1 to 50
		enemy bullets 51 to 100
		enemy = 101

		-> (means check for collisions with)

		//Collisions shouldn't need to be checked both ways

		player->enemy bullets
		enemies->player bullets
		enemies->saber
			//move this to the saber check later	(possibly ditching this because when enemies are checking
			for sabers there are less iterations done, since only active enemies will do this and there
			are only 2 max sabers, whereas a saber will be checking if every enemy is active)
		enemies->light bullets

		saber->light bullets (because this will be cheaper)



		*/


		//Don't forget about these

		for (POOL_PLAYER_LOOP)
		{
			if (!m_entities[i]->isActive) continue;
			CAST_PLAYER(m_entities[i])->CollisionCheck(&m_entities);
		}

		for (POOL_ENEMY_ALL_LOOP)
		{
			if (!m_entities[i]->isActive) continue;
			/*//possible change, store all colliders in a seperate vector and instead check against
			//a collision value to see what it's colliding with
			//problem is, it means more collision checks will have to be done
			//Possible change, each GetCollision function returns a list of all
			//colliders attached to the object, and a collision check is done
			//on each one, and THEN the collision ID is checked
			//for now, keep it like this. Only needed when melee attack function is implemented,
			//but another method would be to have the sword as it's own entity.*/
			CAST_ENEMY(m_entities[i])->CollisionCheck(&m_entities);

		}

		for (POOL_SABER_LOOP)
		{
			if (!m_entities[i]->isActive) continue;
			CAST_SABER(m_entities[i])->CollisionCheck(&m_entities);
		}

		#pragma endregion

		
	}
}

//move this function above the game
void World::LoadMainMenu()
{

	int width{ SCR_WIDTH };
	int height{ SCR_HEIGHT };

	HAPI.SetShowFPS(true);
	int iconW;
	int iconH;
	BYTE* iconData;
	/*if (HAPI.LoadTexture("HAPI/Reference/images/HAPI_Logo220_2.png", &iconData, iconW, iconH))
	{
		HAPI.SetIcon(iconData, iconW, iconH);

	}*/
	if (!HAPI.Initialise(width, height, WINDOW_TITLE))
		return;

	BYTE* screen{ HAPI.GetScreenPointer() };
	visual_menu = LoadGraphicSystem(EGraphicsSystem::eHAPI);
	

	const HAPISPACE::HAPI_TKeyboardData& m_keyboardData(HAPI.GetKeyboardData());
	const HAPISPACE::HAPI_TMouseData& m_mouseData(HAPI.GetMouseData());


	assert(visual_menu != nullptr);

	visual_menu->Init(width, height);

	visual_menu->CreateSprite(NULLTEX_STR, "Graphics/texMissing.png");
	visual_menu->CreateSprite("logo", "Graphics/titleLogo.png");
	visual_menu->CreateSprite("menuBg", "Graphics/menuPlaceholder.png");
	visual_menu->CreateSprite("button", "Graphics/buttonPlaceholder.png");
	visual_menu->CreateSprite("buttonMap", "Graphics/ui_buttons.png", true, "Graphics/UIButtons_list.xml");
	visual_menu->CreateSprite("buttonsArrows", "Graphics/ui_scrollButtons.png", true, "Graphics/UIButtons_Scroll_List.xml");
	visual_menu->CreateSprite("buttonsDifficulty", "Graphics/buttons_difficulty.png", true, "Graphics/difficulty_buttons_sheet.xml");
	visual_menu->CreateSprite("buttonsSpeed", "Graphics/buttons_speed.png", true, "Graphics/difficulty_speed_sheet.xml");
	visual_menu->CreateSprite("checkbox", "Graphics/ui_checkbox.png", true, "Graphics/UICheckbox_List.xml");
	visual_menu->CreateSprite("secretButton", "Graphics/secretButt.png");

	//talk about use of manipulating rects in anim sheet to make simple clipping animations

	std::vector<UIElement*> allUIElements;
	std::vector<UI_Button*> allButtons;


	/*Different Menus:
	* MAIN
	*	GAME SETUP							1
	*		1PLAYER							2
	*		2PLAYER							3
	*			_Difficulty Setting	
	*				_Easy					23
	*				_Medium					24
	*				_Hard					25
	*			_Game Speed
	*				_.25					26
	*				_.5						27
	*				_1						28
	*				_1.5					29
	*				_2						30
	*			_Game start					4
	*			_Back						5
	*		_Back							6
	*	HIGHSCORES							7
	*		_view 2P/1P scores				8
	*		_back							9
	*		_clear records					10
	*	OPTIONS								11
	*		Sfx volume DOWN					12
	*		Sfx volume UP					13
	*		Mx volume DOWN					14
	*		Mx volume UP					15
	*		Control scheme BACK				16
	*		Control scheme FORWARD			17
	*		Stereo audio					18
	*		Show collision boxes			19
	*		Back to main menu				20
	*		Secret							21
	*	TUTORIAL?							
	*	QUIT								22
	* 
	*/


	#pragma region 	create ui objects

	int menuId = 0;
	int currentButtonQuery = 0;
	bool doNotActivateButtons = false;

	UIElement background("menuBg");

	allUIElements.push_back(&background);

	UIElement gameTitle("logo");
	//gameTitle.SetPosition(200, 80);
	allUIElements.push_back(&gameTitle);


	#pragma region setup main panel


	UIElement panel_mainMenu;
	panel_mainMenu.SetBoundingRect(BUTTON_A_RECT);
	panel_mainMenu.SetPosition(SCR_WIDTH / 2 - panel_mainMenu.GetRect().Width() / 2, 380);

	gameTitle.ParentObject(&panel_mainMenu);
	gameTitle.SetLocalPosition(-200, -300);

	//panel_mainMenu.isActive = false;

	//create buttons
	UI_Button button_gameSetup("buttonMap");
	button_gameSetup.commandId = 1;
	button_gameSetup.SetBoundingRect(BUTTON_A_RECT);
	button_gameSetup.GetAnimator()->SetAnimation(*visual_menu, "GameStart");

	allUIElements.push_back(&button_gameSetup);
	allButtons.push_back(&button_gameSetup);

	button_gameSetup.ParentObject(&panel_mainMenu);
	//button_gameStart.SetPosition(SCR_WIDTH / 2 - button_gameStart.GetRect().Width() / 2, 400);
	button_gameSetup.SetLocalPosition(0, 00);


	UI_Button button_highscores("buttonMap");
	button_highscores.commandId = 7;
	button_highscores.SetBoundingRect(BUTTON_A_RECT);
	button_highscores.GetAnimator()->SetAnimation(*visual_menu, "Highscore");
	allUIElements.push_back(&button_highscores);
	allButtons.push_back(&button_highscores);
	button_highscores.ParentObject(&panel_mainMenu);
	button_highscores.SetLocalPosition(0, 80);


	UI_Button button_options("buttonMap");
	button_options.commandId = 11;
	button_options.SetBoundingRect(BUTTON_A_RECT);
	button_options.GetAnimator()->SetAnimation(*visual_menu, "Options");
	allUIElements.push_back(&button_options);
	allButtons.push_back(&button_options);
	button_options.ParentObject(&panel_mainMenu);
	button_options.SetLocalPosition(0, 160);


	UI_Button button_quit("buttonMap");
	button_quit.commandId = 22;
	button_quit.SetBoundingRect(BUTTON_A_RECT);
	button_quit.GetAnimator()->SetAnimation(*visual_menu, "Quit");


	allUIElements.push_back(&button_quit);
	allButtons.push_back(&button_quit);

	button_quit.ParentObject(&panel_mainMenu);
	button_quit.SetLocalPosition(0, 240);

	#pragma endregion

	#pragma region 1P2P select panel
	UIElement panel_1P2Pselect;
	panel_1P2Pselect.SetBoundingRect(Rectangle(0, 10, 0, 500));
	panel_1P2Pselect.SetPosition(SCR_WIDTH / 2 - panel_1P2Pselect.GetRect().Width() / 2, SCR_HEIGHT / 2 - panel_1P2Pselect.GetRect().Height() / 2);
	allUIElements.push_back(&panel_1P2Pselect);

	panel_1P2Pselect.isActive = false;

	UI_Button button_select1P("buttonMap");
	button_select1P.SetBoundingRect(BUTTON_A_RECT);
	button_select1P.commandId = 2;
	button_select1P.GetAnimator()->SetAnimation(*visual_menu, "1Player");

	button_select1P.SetLocalPosition(-96, 40);
	allUIElements.push_back(&button_select1P);
	allButtons.push_back(&button_select1P);
	button_select1P.ParentObject(&panel_1P2Pselect);


	UI_Button button_select2P("buttonMap");
	button_select2P.SetBoundingRect(BUTTON_A_RECT);
	button_select2P.commandId = 3;
	button_select2P.GetAnimator()->SetAnimation(*visual_menu, "2Player");

	button_select2P.SetLocalPosition(-96, 100);
	allUIElements.push_back(&button_select2P);
	allButtons.push_back(&button_select2P);
	button_select2P.ParentObject(&panel_1P2Pselect);

	UI_Button button_playerSelectBack("buttonMap");
	button_playerSelectBack.SetBoundingRect(BUTTON_A_RECT);
	button_playerSelectBack.commandId = 6;
	button_playerSelectBack.GetAnimator()->SetAnimation(*visual_menu, "Back");

	button_playerSelectBack.SetLocalPosition(-96, 175);
	allUIElements.push_back(&button_playerSelectBack);
	allButtons.push_back(&button_playerSelectBack);
	button_playerSelectBack.ParentObject(&panel_1P2Pselect);


	#pragma endregion

	#pragma region setup gameplay panel

	UIElement panel_gameplaySetup;
	panel_gameplaySetup.SetBoundingRect(Rectangle(0, 0, 0, 500));
	panel_gameplaySetup.SetPosition(SCR_WIDTH / 2 - panel_gameplaySetup.GetRect().Width() / 2 - 200, 250);
	allUIElements.push_back(&panel_gameplaySetup);

	panel_gameplaySetup.isActive = false;



	#pragma region difficulty select

	//Easy
	UI_Button button_difficulty_easy("buttonsDifficulty");
	button_difficulty_easy.SetBoundingRect(Rectangle(0, 193, 0, 53));
	button_difficulty_easy.commandId = 23;
	button_difficulty_easy.SetLocalPosition(-30, 0);
	button_difficulty_easy.SetAnimation(*visual_menu, "Idle_Easy");
	button_difficulty_easy.doAnimate = true;
	button_difficulty_easy.doNotHoverHighlight = true;
	allUIElements.push_back(&button_difficulty_easy);
	allButtons.push_back(&button_difficulty_easy);

	button_difficulty_easy.ParentObject(&panel_gameplaySetup);

	//Normal
	UI_Button button_difficulty_normal("buttonsDifficulty");
	button_difficulty_normal.SetBoundingRect(Rectangle(0, 193, 0, 53));
	button_difficulty_normal.commandId = 24;
	button_difficulty_normal.SetLocalPosition(125, 0);
	button_difficulty_normal.SetAnimation(*visual_menu, "Idle_Normal");
	button_difficulty_normal.doAnimate = true;
	button_difficulty_normal.doNotHoverHighlight = true;
	allUIElements.push_back(&button_difficulty_normal);
	allButtons.push_back(&button_difficulty_normal);

	button_difficulty_normal.ParentObject(&panel_gameplaySetup);

	//Difficult
	UI_Button button_difficulty_hard("buttonsDifficulty");
	button_difficulty_hard.SetBoundingRect(Rectangle(0, 193, 0, 53));
	button_difficulty_hard.commandId = 25;
	button_difficulty_hard.SetLocalPosition(280, 0);
	button_difficulty_hard.SetAnimation(*visual_menu, "Idle_Hard");
	button_difficulty_hard.doAnimate = true;
	button_difficulty_hard.doNotHoverHighlight = true;
	allUIElements.push_back(&button_difficulty_hard);
	allButtons.push_back(&button_difficulty_hard);

	button_difficulty_hard.ParentObject(&panel_gameplaySetup);

	switch (gameDifficulty)
	{
	case eDifficulty::eEasy:
		button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNegative;
		button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
		break;
	case eDifficulty::eNormal:
		button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNegative;
		button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
		break;
	case eDifficulty::eHard:
		button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNegative;
		break;
	default:
		break;
	}

	#pragma endregion

	#pragma region speed select


	UI_Button button_speed25("buttonsSpeed");
	button_speed25.SetBoundingRect(Rectangle(0, 77, 0, 25));
	button_speed25.commandId = 26;
	button_speed25.SetLocalPosition(0, 200);
	button_speed25.SetAnimation(*visual_menu, "25p");
	button_speed25.doAnimate = true;
	button_speed25.doNotHoverHighlight = true;
	allUIElements.push_back(&button_speed25);
	allButtons.push_back(&button_speed25);

	button_speed25.ParentObject(&panel_gameplaySetup);


	UI_Button button_speed50("buttonsSpeed");
	button_speed50.SetBoundingRect(Rectangle(0, 77, 0, 25));
	button_speed50.commandId = 27;
	button_speed50.SetLocalPosition(77, 200);
	button_speed50.SetAnimation(*visual_menu, "50p");
	button_speed50.doAnimate = true;
	button_speed50.doNotHoverHighlight = true;
	allUIElements.push_back(&button_speed50);
	allButtons.push_back(&button_speed50);

	button_speed50.ParentObject(&panel_gameplaySetup);

	UI_Button button_speed100("buttonsSpeed");
	button_speed100.SetBoundingRect(Rectangle(0, 77, 0, 25));
	button_speed100.commandId = 28;
	button_speed100.SetLocalPosition(154, 200);
	button_speed100.SetAnimation(*visual_menu, "100p");
	button_speed100.doAnimate = true;
	button_speed100.doNotHoverHighlight = true;
	allUIElements.push_back(&button_speed100);
	allButtons.push_back(&button_speed100);

	button_speed100.ParentObject(&panel_gameplaySetup);

	UI_Button button_speed150("buttonsSpeed");
	button_speed150.SetBoundingRect(Rectangle(0, 77, 0, 25));
	button_speed150.commandId = 29;
	button_speed150.SetLocalPosition(231, 200);
	button_speed150.SetAnimation(*visual_menu, "150p");
	button_speed150.doAnimate = true;
	button_speed150.doNotHoverHighlight = true;
	allUIElements.push_back(&button_speed150);
	allButtons.push_back(&button_speed150);

	button_speed150.ParentObject(&panel_gameplaySetup);

	UI_Button button_speed200("buttonsSpeed");
	button_speed200.SetBoundingRect(Rectangle(0, 77, 0, 25));
	button_speed200.commandId = 30;
	button_speed200.SetLocalPosition(308, 200);
	button_speed200.SetAnimation(*visual_menu, "150p");
	button_speed200.doAnimate = true;
	button_speed200.doNotHoverHighlight = true;
	allUIElements.push_back(&button_speed200);
	allButtons.push_back(&button_speed200);

	button_speed200.ParentObject(&panel_gameplaySetup);



	switch (gameSpeed)
	{
	case eGameSpeed::e25:
		button_speed25.currentSpriteEffectDefault = SpriteEffect::eNegative;
		button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
		break;
	case eGameSpeed::e50:
		button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed50.currentSpriteEffectDefault = SpriteEffect::eNegative;
		button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
		break;
	case eGameSpeed::e100:
		button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed100.currentSpriteEffectDefault = SpriteEffect::eNegative;
		button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
		break;
	case eGameSpeed::e150:
		button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed150.currentSpriteEffectDefault = SpriteEffect::eNegative;
		button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
		break;
	case eGameSpeed::e200:
		button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
		button_speed200.currentSpriteEffectDefault = SpriteEffect::eNegative;
		break;
	default:
		break;
	}




	#pragma endregion



	UI_Button button_gameStart("buttonMap");
	button_gameStart.SetBoundingRect(BUTTON_A_RECT);
	button_gameStart.commandId = 4;
	button_gameStart.SetLocalPosition(220, 450);
	button_gameStart.GetAnimator()->SetAnimation(*visual_menu, "Start");


	allUIElements.push_back(&button_gameStart);
	allButtons.push_back(&button_gameStart);

	button_gameStart.ParentObject(&panel_gameplaySetup);


	UI_Button button_backTo1P2P("buttonMap");
	button_backTo1P2P.SetBoundingRect(BUTTON_A_RECT);
	button_backTo1P2P.commandId = 5;
	button_backTo1P2P.SetLocalPosition(-20 - button_backTo1P2P.GetRect().Width() + 200, 450);
	button_backTo1P2P.GetAnimator()->SetAnimation(*visual_menu, "Back");

	allUIElements.push_back(&button_backTo1P2P);
	allButtons.push_back(&button_backTo1P2P);

	button_backTo1P2P.ParentObject(&panel_gameplaySetup);

	#pragma endregion



	#pragma region Leaderboards menu

	UIElement panel_Leaderboards;
	panel_Leaderboards.SetBoundingRect(Rectangle(0, 0, 0, 500));
	panel_Leaderboards.SetPosition(SCR_WIDTH / 2 - panel_Leaderboards.GetRect().Width() / 2, SCR_HEIGHT / 2 - panel_Leaderboards.GetRect().Height() / 2);
	allUIElements.push_back(&panel_Leaderboards);
	panel_Leaderboards.isActive = false;

	UI_Button button_exitLeaderboard("buttonMap");
	button_exitLeaderboard.SetBoundingRect(BUTTON_A_RECT);
	button_exitLeaderboard.commandId = 9;
	button_exitLeaderboard.GetAnimator()->SetAnimation(*visual_menu, "Back");
	button_exitLeaderboard.SetLocalPosition(-120, 540);
	allUIElements.push_back(&button_exitLeaderboard);
	allButtons.push_back(&button_exitLeaderboard);
	button_exitLeaderboard.ParentObject(&panel_Leaderboards);

	#pragma endregion



	#pragma region Ooptions menu

	UIElement panel_Options;
	panel_Options.SetBoundingRect(Rectangle(0, 10, 0, 500));
	panel_Options.SetPosition(SCR_WIDTH / 2 - panel_Options.GetRect().Width() / 2, SCR_HEIGHT / 2 - panel_Options.GetRect().Height() / 2);
	allUIElements.push_back(&panel_Options);
	panel_Options.isActive = false;

	#pragma region volume sounds

	UI_Button button_sfxVolumeDown("buttonsArrows");
	button_sfxVolumeDown.SetBoundingRect(BUTTON_B_RECT);
	button_sfxVolumeDown.commandId = 12;
	button_sfxVolumeDown.GetAnimator()->SetAnimation(*visual_menu, "Backward");
	button_sfxVolumeDown.SetLocalPosition(-160, 40);
	allUIElements.push_back(&button_sfxVolumeDown);
	allButtons.push_back(&button_sfxVolumeDown);
	button_sfxVolumeDown.ParentObject(&panel_Options);

	button_sfxVolumeDown.allowHold = true;

	UI_Button button_sfxVolumeUp("buttonsArrows");
	button_sfxVolumeUp.SetBoundingRect(BUTTON_B_RECT);
	button_sfxVolumeUp.commandId = 13;
	button_sfxVolumeUp.GetAnimator()->SetAnimation(*visual_menu, "Forward");
	button_sfxVolumeUp.SetLocalPosition(160, 40);
	allUIElements.push_back(&button_sfxVolumeUp);
	allButtons.push_back(&button_sfxVolumeUp);
	button_sfxVolumeUp.ParentObject(&panel_Options);

	button_sfxVolumeUp.allowHold = true;

	#pragma endregion

	#pragma region volume music

	UI_Button button_mxVolumeDown("buttonsArrows");
	button_mxVolumeDown.SetBoundingRect(BUTTON_B_RECT);
	button_mxVolumeDown.commandId = 14;
	button_mxVolumeDown.GetAnimator()->SetAnimation(*visual_menu, "Backward");
	button_mxVolumeDown.SetLocalPosition(-160, 140);
	allUIElements.push_back(&button_mxVolumeDown);
	allButtons.push_back(&button_mxVolumeDown);
	button_mxVolumeDown.ParentObject(&panel_Options);

	button_mxVolumeDown.allowHold = true;

	UI_Button button_mxVolumeUp("buttonsArrows");
	button_mxVolumeUp.SetBoundingRect(BUTTON_B_RECT);
	button_mxVolumeUp.commandId = 15;
	button_mxVolumeUp.GetAnimator()->SetAnimation(*visual_menu, "Forward");
	button_mxVolumeUp.SetLocalPosition(160, 140);
	allUIElements.push_back(&button_mxVolumeUp);
	allButtons.push_back(&button_mxVolumeUp);
	button_mxVolumeUp.ParentObject(&panel_Options);

	button_mxVolumeUp.allowHold = true;

	#pragma endregion

	#pragma region control scheme

	UI_Button button_controlsDown("buttonsArrows");
	button_controlsDown.SetBoundingRect(BUTTON_B_RECT);
	button_controlsDown.commandId = 16;
	button_controlsDown.GetAnimator()->SetAnimation(*visual_menu, "Backward");
	button_controlsDown.SetLocalPosition(-260, 270);
	allUIElements.push_back(&button_controlsDown);
	allButtons.push_back(&button_controlsDown);
	button_controlsDown.ParentObject(&panel_Options);

	UI_Button button_controlsUp("buttonsArrows");
	button_controlsUp.SetBoundingRect(BUTTON_B_RECT);
	button_controlsUp.commandId = 17;
	button_controlsUp.GetAnimator()->SetAnimation(*visual_menu, "Forward");
	button_controlsUp.SetLocalPosition(260, 270);
	allUIElements.push_back(&button_controlsUp);
	allButtons.push_back(&button_controlsUp);
	button_controlsUp.ParentObject(&panel_Options);

	#pragma endregion


	UI_Button button_toggleStereo("checkbox");
	button_toggleStereo.SetBoundingRect(BUTTON_A_RECT);
	button_toggleStereo.commandId = 18;
	//depend on current state
	if (stereoOn)
	{
		button_toggleStereo.GetAnimator()->SetAnimation(*visual_menu, "Filled");
	}
	else
	{
		button_toggleStereo.GetAnimator()->SetAnimation(*visual_menu, "Empty");
	}
	button_toggleStereo.SetLocalPosition(160, 370);
	allUIElements.push_back(&button_toggleStereo);
	allButtons.push_back(&button_toggleStereo);
	button_toggleStereo.ParentObject(&panel_Options);

	UI_Button button_toggleCollidersDebug("checkbox");
	button_toggleCollidersDebug.SetBoundingRect(BUTTON_A_RECT);
	button_toggleCollidersDebug.commandId = 19;
	//depend on current state
	if (collidersOn)
	{
		button_toggleCollidersDebug.GetAnimator()->SetAnimation(*visual_menu, "Filled");
	}
	else
	{
		button_toggleCollidersDebug.GetAnimator()->SetAnimation(*visual_menu, "Empty");
	}
	button_toggleCollidersDebug.SetLocalPosition(160, 440);
	allUIElements.push_back(&button_toggleCollidersDebug);
	allButtons.push_back(&button_toggleCollidersDebug);
	button_toggleCollidersDebug.ParentObject(&panel_Options);


	UI_Button button_exitOptions("buttonMap");
	button_exitOptions.SetBoundingRect(BUTTON_A_RECT);
	button_exitOptions.commandId = 20;
	button_exitOptions.GetAnimator()->SetAnimation(*visual_menu, "Back");
	button_exitOptions.SetLocalPosition(-120, 540);
	allUIElements.push_back(&button_exitOptions);
	allButtons.push_back(&button_exitOptions);
	button_exitOptions.ParentObject(&panel_Options);

	UI_Button button_secret("secretButton");
	button_secret.SetBoundingRect(Rectangle(0, 57, 0, 46));
	button_secret.commandId = 21;
	button_secret.SetLocalPosition(455, -140);
	allUIElements.push_back(&button_secret);
	allButtons.push_back(&button_secret);
	button_secret.ParentObject(&panel_Options);



	#pragma endregion



	#pragma endregion

	//set mouse data for all buttons
	for (UI_Button* &b : allButtons)
	{
		b->SetMouseData(&m_mouseData);
		b->sfxVolume = &m_sfxVolume;
	}


	eMenuType currentMenuType = eMenuType::eMain;

	

	float lastTick = (float)HAPI.GetTime();
	float dt = 0;

	while (HAPI.Update())
	{

		dt = ((float)HAPI.GetTime() - lastTick) / 1000;
		lastTick = (float)HAPI.GetTime();

		visual_menu->ClearScreen(100, 55, 100);

		//panel_mainMenu.Translate(0, -1);
	
		for (UIElement*& element : allUIElements)
		{
			element->Update(dt);
			element->Render(*visual_menu);
		}

		HAPI.ChangeFontFromFile("Data/Fonts/Pixellari.ttf");
		//HAPI.RenderText(250, 200, HAPI_TColour(255, 0, 0), "Placeholder text", 50);
		//HAPI.ChangeFont

		switch (currentMenuType)
		{
		case eMenuType::eMain: //Main menu (blank?)
		{

			break;
		}
		case eMenuType::e1P2Pselect: //p1p2 select (blank?)
		{
			HAPI.RenderText(390, 110, UI_TEXTCOL_BRIGHT, "MODE SELECT", 40);
			break;
		}
		case eMenuType::eGameSetup: //game setup 
		{
			HAPI.RenderText(400, 185, UI_TEXTCOL_BRIGHT, "DIFFICULTY", 40);
			HAPI.RenderText(390, 385, UI_TEXTCOL_BRIGHT, "GAME SPEED", 40);
			break;
		}
		case eMenuType::eHighScores:
		{
			HAPI.RenderText(400, 185, UI_TEXTCOL_BRIGHT, "coming soon", 40);
			break;
		}
		case eMenuType::eOptions: //options
		{
			HAPI.RenderText(340, 135, UI_TEXTCOL_BRIGHT, "Sound Effect Volume", 40);
			HAPI.RenderText(500, 180, UI_TEXTCOL_BASIC, std::to_string(m_sfxVolume), 40);
			HAPI.RenderText(400, 235, UI_TEXTCOL_BRIGHT, "Music Volume", 40);
			HAPI.RenderText(500, 280, UI_TEXTCOL_BASIC, std::to_string(m_mxVolume), 40);

			HAPI.RenderText(400, 365, UI_TEXTCOL_BRIGHT, "Control Options", 40);
			std::string controlOptions = "_";

			switch (controlSettingsID)
			{
			case 0:
			{
				controlOptions = "1P Keyboard - 2P Controller";
				break;
			}
			case 1:
			{
				controlOptions = "1P Controller - 2P Keyboard";
				break;
			}
			case 2:
			{
				controlOptions = "1P Controller - 2P Controller";
				break;
			}
			default:
				break;
			}

			HAPI.RenderText(300, 410, UI_TEXTCOL_BASIC, controlOptions, 38);



			HAPI.RenderText(300, 510, UI_TEXTCOL_BRIGHT, "Stereo Audio", 38);
			HAPI.RenderText(300, 570, UI_TEXTCOL_BRIGHT, "Debug Hitboxes", 38);

			

			break;
		}
		default:
		{
			break;
		}
		}


		if (!doNotActivateButtons)
		{
			for (UI_Button*& b : allButtons)
			{
				if (!b->isHierarchilyActive()) continue;

				if (b->isPressed)
				{
					
					currentButtonQuery = b->commandId;
					break;
				}
			}
		}
		



		switch (currentButtonQuery)
		{
		case 0: //Do nothing
		{
			
			break;
		}
		case 1: //Start game setup
		{
			
			panel_mainMenu.isActive = false;
			panel_1P2Pselect.isActive = true;
			//doNotActivateButtons = true;
			currentMenuType = eMenuType::e1P2Pselect;

			break;
		}
		case 2: //Select 1P
		{
			panel_1P2Pselect.isActive = false;
			panel_gameplaySetup.isActive = true;
			m_isLocalMultiplayerGame = false;
			currentMenuType = eMenuType::eGameSetup;

			break;
		}
		case 3: //Select 2P
		{
			panel_1P2Pselect.isActive = false;
			panel_gameplaySetup.isActive = true;
			m_isLocalMultiplayerGame = true;
			currentMenuType = eMenuType::eGameSetup;

			break;
		}
		case 4: //RUN GAME (big ting)
		{
			eGameReturnQ state = eGameReturnQ::eRestart;

			while (state == eGameReturnQ::eRestart)
			{
				m_entities.clear();
				m_allInputHandlers.clear();
				state = Run();
			}

			if (state == eGameReturnQ::eQuitGame)
			{
				//HAPI quit function?
				return;
			}
			else if (state == eGameReturnQ::eMenu)
			{
				//panel_gameplaySetup.isActive = false;
				//panel_mainMenu.isActive = true;

				//Step1
				//panel_gameplaySetup.isActive = false;
				//panel_1P2Pselect.isActive = true;
				////Step2
				//panel_1P2Pselect.isActive = false;
				//panel_gameplaySetup.isActive = false;
				//panel_1P2Pselect.isActive = false;
				//panel_mainMenu.isActive = true;

				//currentMenuType = eMenuType::eMain;

				currentButtonQuery = 0;

				/*for (auto& h : allButtons)
				{
					h->EmergencyReset();
				}*/
			}

			//Should be able to return, or should delete all the menu stuff?
			//Handle game results
			//return;
			

			break;
		}
		case 5: //Back to 1P2P menu
		{
			panel_gameplaySetup.isActive = false;
			panel_1P2Pselect.isActive = true;
			currentMenuType = eMenuType::e1P2Pselect;

			break;
		}
		case 6: //Back to main menu
		{
			
			panel_1P2Pselect.isActive = false;
			panel_gameplaySetup.isActive = false;
			panel_1P2Pselect.isActive = false;
			panel_mainMenu.isActive = true;

			currentMenuType = eMenuType::eMain;



			break;
		}

		case 7: //Open highscore menu
		{	
			panel_mainMenu.isActive = false;
			panel_Leaderboards.isActive = true;


			currentMenuType = eMenuType::eHighScores;

			//Others

			break;
		}
		case 8: //Toggle viewing 1 player and 2 player scores
		{

			//Others

			break;
		}
		case 9: //Back to main menu from highscore boards
		{
			panel_Leaderboards.isActive = false;
			panel_mainMenu.isActive = true;
			currentMenuType = eMenuType::eMain;

			break;
		}
		case 10: //Erase records
		{

			//Others

			break;
		}
		case 11: //Open Option Menu
		{
			panel_mainMenu.isActive = false;
			panel_Options.isActive = true;

			currentMenuType = eMenuType::eOptions;

			break;
		}
		case 12: //Sound down
		{
			m_sfxVolume--;
			m_sfxVolume = std::max(m_sfxVolume, 0);

			break;
		}
		case 13: //Sound up
		{
			m_sfxVolume++;
			m_sfxVolume = std::min(m_sfxVolume, 100);


			break;
		}
		case 14: //music down
		{
			m_mxVolume--;
			m_mxVolume = std::max(m_mxVolume, 0);


			break;
		}
		case 15: //music up
		{
			m_mxVolume++;
			m_mxVolume = std::min(m_mxVolume, 100);

			break;
		}
		case 16: //control options down
		{
			controlSettingsID--;
			if (controlSettingsID < 0) controlSettingsID = 2;


			break;
		}
		case 17: //control options up
		{
			controlSettingsID++;
			if (controlSettingsID > 2) controlSettingsID = 0;

			break;
		}
		case 18: //toggle stereo
		{
			stereoOn = !stereoOn;

			if (stereoOn)
			{
				button_toggleStereo.GetAnimator()->SetAnimation(*visual_menu, "Filled");
			}
			else
			{
				button_toggleStereo.GetAnimator()->SetAnimation(*visual_menu, "Empty");
			}


			break;
		}
		case 19: //toggle stereo
		{
			collidersOn = !collidersOn;

			if (collidersOn)
			{
				button_toggleCollidersDebug.GetAnimator()->SetAnimation(*visual_menu, "Filled");
			}
			else
			{
				button_toggleCollidersDebug.GetAnimator()->SetAnimation(*visual_menu, "Empty");
			}


			break;
		}
		case 20: //Exit Options menu
		{
			panel_mainMenu.isActive = true;
			panel_Options.isActive = false;

			currentMenuType = eMenuType::eMain;

			break;
		}
		case 21: //le secret
		{
			if (!HAPI.PlayStreamedMedia("Data/Other/"))
			{
				HAPI.UserMessage("video not found", "le");
			}
			break;
		}
		case 22: //Quit
		{

			return;

			break;
		}
		case 23: //game easy
		{
			gameDifficulty = eDifficulty::eEasy;

			switch (gameDifficulty)
			{
			case eDifficulty::eEasy:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNegative;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
				break;
			case eDifficulty::eNormal:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNegative;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
				break;
			case eDifficulty::eHard:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNegative;
				break;
			default:
				break;
			}

			break;
		}
		case 24: //game normal
		{
			gameDifficulty = eDifficulty::eNormal;

			switch (gameDifficulty)
			{
			case eDifficulty::eEasy:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNegative;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
				break;
			case eDifficulty::eNormal:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNegative;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
				break;
			case eDifficulty::eHard:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNegative;
				break;
			default:
				break;
			}

			break;
		}
		case 25: //game hard
		{
			gameDifficulty = eDifficulty::eHard;

			switch (gameDifficulty)
			{
			case eDifficulty::eEasy:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNegative;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
				break;
			case eDifficulty::eNormal:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNegative;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNone;
				break;
			case eDifficulty::eHard:
				button_difficulty_easy.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_normal.currentSpriteEffectDefault = SpriteEffect::eNone;
				button_difficulty_hard.currentSpriteEffectDefault = SpriteEffect::eNegative;
				break;
			default:
				break;
			}

			break;
		}
		case 26:
		{
			gameSpeed = eGameSpeed::e25;		
			break;
		}
		case 27:
		{
			gameSpeed = eGameSpeed::e50;
			break;
		}
		case 28:
		{
			gameSpeed = eGameSpeed::e100;
			break;
		}
		case 29:
		{
			gameSpeed = eGameSpeed::e150;
			break;
		}
		case 30:
		{
			gameSpeed = eGameSpeed::e200;
			break;
		}
		default:
			break;
		}




		switch (gameSpeed)
		{
		case eGameSpeed::e25:
			button_speed25.currentSpriteEffectDefault = SpriteEffect::eNegative;
			button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
			break;
		case eGameSpeed::e50:
			button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed50.currentSpriteEffectDefault = SpriteEffect::eNegative;
			button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
			break;
		case eGameSpeed::e100:
			button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed100.currentSpriteEffectDefault = SpriteEffect::eNegative;
			button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
			break;
		case eGameSpeed::e150:
			button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed150.currentSpriteEffectDefault = SpriteEffect::eNegative;
			button_speed200.currentSpriteEffectDefault = SpriteEffect::eNone;
			break;
		case eGameSpeed::e200:
			button_speed25.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed50.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed100.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed150.currentSpriteEffectDefault = SpriteEffect::eNone;
			button_speed200.currentSpriteEffectDefault = SpriteEffect::eNegative;
			break;
		default:
			break;
		}



		currentButtonQuery = 0;





		
		
	}
}
