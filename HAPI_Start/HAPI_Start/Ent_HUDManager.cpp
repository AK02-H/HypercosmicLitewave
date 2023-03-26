#include "Ent_HUDManager.h"

void Ent_HUDManager::Update(float dt)
{
	for (size_t i = 0; i < 100; i++)
	{
		if (!popups_actives[i]) continue;
		

		//Text grows from being small when created
		if (popups_sizes_grow[i] < popups_sizes_max[i])
		{
			popups_sizes_grow[i] += dt * k_textGrowSpeed;

			if (popups_sizes_grow[i] > popups_sizes_max[i])
			{
				popups_sizes_grow[i] = (int)popups_sizes_max[i];
			}
		}
		


		timers[i] += dt;
		if (timers[i] > durations[i])
		{
			popups_isFading[i] = true;
			//popups_actives[i] = false;
		}


		if (popups_isFading[i])
		{
			int newAlpha = colours[i].alpha - dt * k_textFadeSpeed;
			//HAPI_TColour newCol(colours[i].red, colours[i].)
			if (newAlpha < 0) newAlpha = 0;
			colours[i].alpha = newAlpha;

			if (colours[i].alpha <= 0)
			{
				colours[i] = 0;
				popups_actives[i] = false;
				popups_isFading[i] = false;
			}

		}

	}

	debugTimer += dt;


}

void Ent_HUDManager::Render(IGraphics& viz)
{
	//Render score in top corner
	//Render lives in top corner

	for (size_t i = 0; i < 100; i++)
	{
		if (!popups_actives[i]) continue;
   		HAPI.RenderText(positions[i].x, positions[i].y, colours[i], popups_text[i], popups_sizes_grow[i]);
		//HAPI.RenderText()

	}

	//Render score
	if (scorePtr != nullptr)
	{
		HAPI.RenderText(0, 0, HAPI_TColour::GREEN, std::to_string(*scorePtr), 40);
	}

	//Render lives
	if (lifeCountPtr_1 != nullptr)
	{
		HAPI.RenderText(0, 50, HAPI_TColour::GREEN, "x " + std::to_string(*lifeCountPtr_1), 25);
	}

	// Render lives
	
	if (isMultiplayerGame)
	{
		if (lifeCountPtr_2 != nullptr)
		{
			HAPI.RenderText(SCR_WIDTH - 50, 50, HAPI_TColour::GREEN, "x " + std::to_string(*lifeCountPtr_2), 25);
		}
	}
	
	HAPI.RenderText(0, SCR_HEIGHT - 50, HAPI_TColour::CYAN, "TIME: " + std::to_string(debugTimer), 25);
	//timer
}

void Ent_HUDManager::CreatePopupText(std::string text, Vector2 position, float duration, HAPI_TColour col, int size)
{
	//
	for (size_t i = 0; i < 100; i++)
	{
		if (popups_actives[i]) continue; //find first inactive text

		//HAPI_TColour newCol(255, 0, 0, 255);	//change

		popups_text[i] = text;
		positions[i] = position;
		colours[i] = col;
		popups_sizes_grow[i] = 1;
		popups_sizes_max[i] = size;
		timers[i] = 0;
		durations[i] = duration;

		popups_actives[i] = true;

		popups_isFading[i] = false;

		return;
	}
}
