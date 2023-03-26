#pragma once
#include "Entity.h"
class Ent_HUDManager :
    public Entity
{
private:

	int* scorePtr{ nullptr };
	int* lifeCountPtr_1{ nullptr };
	int* lifeCountPtr_2{ nullptr };

	bool isMultiplayerGame = false;

	const int k_maxPoolSize = 100;
	const float k_textGrowSpeed = 100;
	const float k_textFadeSpeed = 550;

	std::string popups_text[100] = {""};
	bool popups_actives[100] = {false};
	bool popups_isFading[100] = {false};
	float popups_sizes_grow[100] = {0};
	int popups_sizes_max[100] = {0};
	Vector2 positions[100];
	HAPI_TColour colours[100];

	float timers[100] = { 0 };
	float durations[100] = { 0 };

	float debugTimer = 0;

public:

	Ent_HUDManager()
	{

	}

	void Load() override final {};

	void Update(float dt) override final;
	void Render(IGraphics& viz);
	void LateUpdate(float dt) override final{};

	void SetIsMultiplayerGame(bool is) { isMultiplayerGame = is; }

	void SetScorePtr(int* ptr)
	{
		scorePtr = ptr;
	}

	void SetLifePtr_1(int* ptr)
	{
		lifeCountPtr_1 = ptr;
	}

	void SetLifePtr_2(int* ptr)
	{
		lifeCountPtr_2 = ptr;
	}

	void CreatePopupText(std::string text, Vector2 position, float duration, HAPI_TColour col = UI_TEXTCOL_BRIGHT, int size = 30);
	
};

