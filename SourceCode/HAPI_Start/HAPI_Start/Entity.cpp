#include "Entity.h"

Entity* Entity::RetrieveEntity(std::vector<Entity*>* list, int rangeA, int rangeB, bool activeOnRetrieve)
{
	//fire method:
		//loop through bullet range of entity pool
		//find first inactive bullet
		//prime bullet and set it active
		//if no active bullet is found, use first bullet in pool regardless of activity

	Entity* toReturn{ nullptr };
	bool hasFound{ false };

	for (size_t i = rangeA; i < rangeB; i++)
	{
		if (!(*list)[i]->isActive)
		{
			toReturn = (*list)[i];
			//Activate entity in this code?
			if (activeOnRetrieve) toReturn->SetActive(true);
			hasFound = true;

			break;
		}
	}

	if (!hasFound)
	{
		toReturn = (*list)[rangeA];
	}

	return toReturn;
}

void Entity::Render(IGraphics& viz) {
	viz.DrawSprite(m_gfxName, m_posX, m_posY);
}

float Entity::RelativeScreenX()
{
	//Currently ONLY used for sound panning which is why I can do this here
	if (!USE_STEREO_AUDIO) return 0;

	float value = (float)m_posX / (float)SCR_WIDTH;
	value = value * 2;
	value = 1 - value;
	value *= -1;
	if (value > 1) value = 1;
	if (value < -1) value = -1;
	return value;
}
