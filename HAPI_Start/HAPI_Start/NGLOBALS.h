#pragma once

//inline float g_timescale{ 1.0f };	//allows timescale to be modified
static float g_timescale{ 1.0f };	//allows timescale to be modified
//only instance of dangerous global variable

enum class SpriteEffect
{
	eNone,
	eFlashWhiteFull,
	eFlashWhitePartial,
	eNegative,
	eHalfAlpha,
	eQuarterAlpha,
	eBoostBlue,
	eNegativeHalfTransparent
};

enum class eScrollType
{
	Horizontal,
	Vertical,
	HorizontalAndVertical
};

enum class eScrollDirection
{
	Forward,
	Backward,
	BothWays,	//doesn't work
	//These values are specifically when scroll type is both
	HorizFOR_Vert_FOR,
	HorizFOR_Vert_BACK,
	HorizBACK_Vert_FOR,
	HorizBACK_Vert_BACK,
};

enum class eSFX
{
	ePlayerFire,
	ePlayerSlash,
	ePlayerSlashHyper,
	ePlayerChargeReady,
	eExplosion_1,
	eDeflect,
	eEnemyFire,
	ePlayerDie,
	ePlayerRespawn,
	eNuke,
	eNukeDie,
	ekamiSpawn,
	ekamiLaunch,
};

class Chron
{
public:

	float timescale{ 1.0f };
	float timeBASE{ 1.0f };

	static Chron& Chron::GetInstance()
	{
		static Chron instance;
		return instance;
	}
};

class SauceyGlobals
{
public:

	bool debugColliders = false;
	bool stereoAudio = true;
	float difficultyScale = 1;
	int maxLives = 5;

	static SauceyGlobals& SauceyGlobals::GetInstance()
	{
		static SauceyGlobals instance;
		return instance;
	}
};

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768
#define WINDOW_TITLE "HCLW"
//For placing things relative to screen size
//preferably float inputs
#define SCR_PLACEHORIZ(percentage) (int) (SCR_WIDTH * percentage)
#define SCR_PLACEVERT(percentage) (int) (SCR_HEIGHT * percentage)


#define PLAYER_MIN_X -55
#define PLAYER_MAX_X SCR_WIDTH - (125 + PLAYER_MIN_X)
#define PLAYER_MIN_Y -50
#define PLAYER_MAX_Y SCR_HEIGHT - 60

#define ENEMYSCREENBOUND_MIN_X -100
#define ENEMYSCREENBOUND_MAX_X SCR_WIDTH
#define ENEMYSCREENBOUND_MIN_Y -100
#define ENEMYSCREENBOUND_MAX_Y SCR_HEIGHT

#define BULLETBOUND_MIN_X - 250
#define BULLETBOUND_MAX_X SCR_WIDTH + 250
#define BULLETBOUND_MIN_Y - 250
#define BULLETBOUND_MAX_Y SCR_HEIGHT + 550

#define PLAYER1_SPAWNPOS SCR_PLACEHORIZ(0.33f), SCR_PLACEVERT(0.72f)
#define PLAYER2_SPAWNPOS SCR_PLACEHORIZ(0.53f), SCR_PLACEVERT(0.72f)

//#define isnt null != nullptr

//UI
#define BUTTON_A_RECT Rectangle(0, 245, 0, 61)
#define BUTTON_B_RECT Rectangle(0, 39, 0, 56)
#define BUTTON_C_RECT Rectangle(0, 225, 0, 61)


#define UI_TEXTCOL_BASIC HAPI_TColour(227, 178, 255)
#define UI_TEXTCOL_BRIGHT HAPI_TColour(219, 65, 195)
#define UI_TEXTCOL_LIGHT HAPI_TColour(255, 243, 146)



#define UPSCALE 1

#define NULLTEX_STR "nulltex"

#define PI 3.14159265358979323846
//#define TIMESCALE g_timescale
#define TIMESCALE Chron::GetInstance().timescale
//To free an entity from timescaling, divide deltatimed processes by TIMESCALE
#define DEBUG_COLLIDERS SauceyGlobals::GetInstance().debugColliders
#define USE_STEREO_AUDIO SauceyGlobals::GetInstance().stereoAudio
#define LEVEL_SCALE SauceyGlobals::GetInstance().difficultyScale
#define PLAYER_START_LIVES SauceyGlobals::GetInstance().maxLives


//shortcuts
#define CAST_PLAYER(ptr) static_cast<Ent_Player*>(ptr)
#define CAST_ENEMY(ptr) static_cast<Ent_Enemy*>(ptr)
#define CAST_BULLET(ptr) static_cast<Ent_Bullet*>(ptr)
#define CAST_SABER(ptr) static_cast<Ent_PlayerSaber*>(ptr)
#define CAST_BULLET_LIGHT(ptr) static_cast<Ent_Bullet_Light*>(ptr)
#define CAST_EXPLOSION(ptr) static_cast<Ent_Explosion*>(ptr)

//damageValues
#define SABER_BASEDAMAGE 50
#define SABER_HYPERDAMAGE 100
#define LIGHTBULLET_BASEDAMAGE 70
#define LIGHTBULLET_HYPERDAMAGEMULTIPLIER 1.5f
#define LIGHTBULLET_HYPERSPEEDMULTIPLIER 3.2f
#define LIGHTBULLET_HYPEREXTRAWALLBOUNCES 3.0f
#define LIGHTBULLET_SCOREMULTIPLIER 1.75f

//enemy stats
#define NME_BASIC_MAXHP	30 / (LEVEL_SCALE)
#define NME_BASIC_SCORE	250
#define NME_BASIC_FIREINTERVAL 1.1f * LEVEL_SCALE
#define NME_BASIC_INITIALFIREDELAY 0.5f * LEVEL_SCALE


#define NME_VERTICAL_MAXHP 85 / (LEVEL_SCALE)
#define NME_VERTICAL_SCORE 600
#define NME_VERTICAL_FIREINTERVAL 0.9f * LEVEL_SCALE
#define NME_VERTICAL_INITIALFIREDELAY 0.0f * LEVEL_SCALE

#define NME_BURST_MAXHP	300
#define NME_BURST_SCORE	1000
#define NME_BURST_FIREINTERVAL 0.0f * LEVEL_SCALE
#define NME_BURST_INITIALFIREDELAY 0.0f * LEVEL_SCALE

#define NME_KAMIKAZE_MAXHP 90 / (LEVEL_SCALE)
#define NME_KAMIKAZE_SCORE 450
#define NME_KAMIKAZE_FIREINTERVAL 0.0f * LEVEL_SCALE
#define NME_KAMIKAZE_INITIALFIREDELAY 0.0f * LEVEL_SCALE

#define NME_MISSILE_MAXHP 500  / (LEVEL_SCALE)		//big enemies don't have difficulty hp scale applied to them
#define NME_MISSILE_SCORE 1500
#define NME_MISSILE_FIREINTERVAL 0.0f * LEVEL_SCALE
#define NME_MISSILE_INITIALFIREDELAY 0.0f * LEVEL_SCALE

//animated entity framerates
#define FR_PLAYER 10
#define FR_EXPLOSION_1 45
#define FR_EXPLOSION_2 60
#define FR_BULLET_LIGHT 30

#define FR_ENEMY_KAMIKAZE 20
#define FR_ENEMY_MISSILE 30


//Enemy hitbox data
#define HB_EN_BASIC					0, 34, 0, 36
#define HB_EN_BASIC_OFFSET			{0, 0}
#define HB_EN_VERTICAL				0, 54, 0, 38
#define HB_EN_VERTICAL_OFFSET		{0, 16}
#define HB_EN_BURST					0, 34, 0, 36
#define HB_EN_BURST_OFFSET			{0, 0}
#define HB_EN_KAMIKAZE				0, 27, 0, 27
#define HB_EN_KAMIKAZE_OFFSET		{18, 18}
#define HB_EN_MISSILE				0, 55, 0, 328
#define HB_EN_MISSILE_OFFSET		{18, 13}


//Pool ranges (PER, Pool Entity Range)
//All these objects must be created and pushed to m_entities (in this SAME order) or the game will crash

#define ENEMYMANAGER_COUNT 1
#define PER_ENEMYMANAGER_START 0
#define PER_ENEMYMANAGER_END PER_ENEMYMANAGER_START + ENEMYMANAGER_COUNT

#define AUDIOMANAGER_COUNT 1
#define PER_AUDIOMANAGER_START PER_ENEMYMANAGER_END
#define PER_AUDIOMANAGER_END PER_AUDIOMANAGER_START + AUDIOMANAGER_COUNT

#define BACKGROUND_COUNT 1
#define PER_BACKGROUND_START PER_AUDIOMANAGER_END
#define PER_BACKGROUND_END PER_BACKGROUND_START + BACKGROUND_COUNT



#define SABER_COUNT 2
#define PER_SABER_START PER_BACKGROUND_END
#define PER_SABER_END PER_SABER_START + SABER_COUNT
#define SABER_RANGE PER_SABER_START, PER_SABER_END
#define POOL_SABER_LOOP size_t i = PER_SABER_START; i < PER_SABER_END; i++

#define PLAYER_COUNT 2
#define PER_PLAYER_START PER_SABER_END
#define PER_PLAYER_END PER_PLAYER_START + PLAYER_COUNT
#define PLAYER_RANGE PER_PLAYER_START, PER_PLAYER_END
#define POOLSIZE_PLAYER PER_PLAYER_END - PER_PLAYER_START
#define POOL_PLAYER_LOOP size_t i = PER_PLAYER_START; i < PER_PLAYER_END; i++


#define BULLETSPLAYER_COUNT				50
#define PER_BULLETSPLAYER_START			PER_PLAYER_END	
#define PER_BULLETSPLAYER_END			PER_BULLETSPLAYER_START + BULLETSPLAYER_COUNT
#define BULLETSPLAYER_RANGE				PER_BULLETSPLAYER_START, PER_BULLETSPLAYER_END
#define POOL_BULLETSPLAYER_LOOP			size_t i = PER_BULLETSPLAYER_START; i < PER_BULLETSPLAYER_END; i++

#define BULLETSPLAYER_B_COUNT			50
#define PER_BULLETSPLAYER_B_START		PER_BULLETSPLAYER_END	
#define PER_BULLETSPLAYER_B_END			PER_BULLETSPLAYER_B_START + BULLETSPLAYER_B_COUNT
#define BULLETSPLAYER_B_RANGE			PER_BULLETSPLAYER_B_START, PER_BULLETSPLAYER_B_END
#define POOL_BULLETSPLAYER_B_LOOP		size_t i = PER_BULLETSPLAYER_B_START; i < PER_BULLETSPLAYER_B_END; i++


//BULLETS AND TYPES

#define PER_BULLETS_ALL_START			PER_BULLETSPLAYER_B_END

#pragma region bullet subgroups

//ENEMY BULLETS TPYE A
#define BULLETS_ENEMY_A_COUNT			500
#define PER_BULLETS_ENEMY_A_START		PER_BULLETS_ALL_START	//51
#define PER_BULLETS_ENEMY_A_END			PER_BULLETS_ENEMY_A_START + BULLETS_ENEMY_A_COUNT	//101
#define BULLETS_ENEMY_A_RANGE			PER_BULLETS_ENEMY_A_START, PER_BULLETS_ENEMY_A_END
#define POOL_BULLETS_ENEMY_A_LOOP		size_t i = PER_BULLETS_ENEMY_A_START; i < PER_BULLETS_ENEMY_A_END; i++

//LIGHTWAVE BULLETS
#define BULLETS_LIGHT_COUNT				250 
#define PER_BULLETS_LIGHT_START			PER_BULLETS_ENEMY_A_END
#define PER_BULLETS_LIGHT_END			PER_BULLETS_LIGHT_START + BULLETS_LIGHT_COUNT
#define BULLETS_LIGHT_RANGE				PER_BULLETS_LIGHT_START, PER_BULLETS_LIGHT_END
#define POOL_BULLETS_LIGHT_LOOP			size_t i = PER_BULLETS_LIGHT_START; i < PER_BULLETS_LIGHT_END; i++

#pragma endregion

#define PER_BULLETS_ALL_END				PER_BULLETS_ALL_START + BULLETS_ENEMY_A_COUNT + BULLETS_LIGHT_COUNT
#define BULLETS_ALL_RANGE				PER_BULLETS_ALL_START, PER_BULLETS_ALL_END
#define POOL_BULLETS_ALL_LOOP			size_t i = PER_BULLETS_ALL_START; i < PER_BULLETS_ALL_END; i++



//have ranges within ranges for different types of enemies

#define PER_ENEMY_ALL_START				PER_BULLETS_ALL_END

#pragma region enemy subgroups

//there is currently no need to loop over individual enemy types

//Basic enemy (fire in direction of player)
#define ENEMY_BASIC_COUNT				50
#define PER_ENEMY_BASIC_START			PER_BULLETS_ALL_END
#define PER_ENEMY_BASIC_END				PER_ENEMY_BASIC_START + ENEMY_BASIC_COUNT
#define ENEMY_BASIC_RANGE				PER_ENEMY_BASIC_START, PER_ENEMY_BASIC_END

//Vertical enemy (fires directly downwards)
#define ENEMY_VERTICAL_COUNT			30
#define PER_ENEMY_VERTICAL_START		PER_ENEMY_BASIC_END
#define PER_ENEMY_VERTICAL_END			PER_ENEMY_VERTICAL_START + ENEMY_VERTICAL_COUNT
#define ENEMY_VERTICAL_RANGE			PER_ENEMY_VERTICAL_START, PER_ENEMY_VERTICAL_END

//Burst enemy (waits then launches pattern)
#define ENEMY_BURST_COUNT				10
#define PER_ENEMY_BURST_START			PER_ENEMY_VERTICAL_END
#define PER_ENEMY_BURST_END				PER_ENEMY_BURST_START + ENEMY_BURST_COUNT
#define ENEMY_BURST_RANGE				PER_ENEMY_BURST_START, PER_ENEMY_BURST_END

//Kamikaze ship (spawns then flies toward player)
#define ENEMY_KAMIKAZE_COUNT			30
#define PER_ENEMY_KAMIKAZE_START		PER_ENEMY_BURST_END
#define PER_ENEMY_KAMIKAZE_END			PER_ENEMY_KAMIKAZE_START + ENEMY_KAMIKAZE_COUNT
#define ENEMY_KAMIKAZE_RANGE			PER_ENEMY_KAMIKAZE_START, PER_ENEMY_KAMIKAZE_END

//Missile (moves directly downwards and explodes when reaching bottom of screen)
#define ENEMY_MISSILE_COUNT				20
#define PER_ENEMY_MISSILE_START			PER_ENEMY_KAMIKAZE_END
#define PER_ENEMY_MISSILE_END			PER_ENEMY_MISSILE_START + ENEMY_MISSILE_COUNT
#define ENEMY_MISSILE_RANGE				PER_ENEMY_MISSILE_START, PER_ENEMY_MISSILE_END

#pragma endregion

#define PER_ENEMY_ALL_END				PER_ENEMY_ALL_START + ENEMY_BASIC_COUNT + ENEMY_VERTICAL_COUNT + ENEMY_BURST_COUNT + ENEMY_KAMIKAZE_COUNT + ENEMY_MISSILE_COUNT
#define ENEMY_ALL_RANGE					PER_ENEMY_ALL_START, PER_ENEMY_ALL_END
#define POOL_ENEMY_ALL_LOOP				size_t i = PER_ENEMY_ALL_START; i < PER_ENEMY_ALL_END; i++


#define EXPLOSION_COUNT					80
#define PER_EXPLOSION_START				PER_ENEMY_ALL_END 
#define PER_EXPLOSION_END				PER_EXPLOSION_START + EXPLOSION_COUNT
#define EXPLOSION_RANGE					PER_EXPLOSION_START, PER_EXPLOSION_END
#define POOL_EXPLOSION_LOOP				size_t i = PER_EXPLOSION_START; i < PER_EXPLOSION_END; i++

#define EXPLOSION_TYPEB_COUNT			5
#define PER_EXPLOSION_TYPEB_START		PER_EXPLOSION_END 
#define PER_EXPLOSION_TYPEB_END			PER_EXPLOSION_TYPEB_START + EXPLOSION_TYPEB_COUNT
#define EXPLOSION_TYPEB_RANGE			PER_EXPLOSION_TYPEB_START, PER_EXPLOSION_TYPEB_END
#define POOL_EXPLOSION_TYPEB_LOOP		size_t i = PER_EXPLOSION_TYPEB_START; i < PER_EXPLOSION_TYPEB_END; i++