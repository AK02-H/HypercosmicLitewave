#include "Collider.h"

bool Collider::OverlapsRect(Collider* other)
{
	//check own collider type
	switch (colliderType)
	{
	case EColliderType::eBox:

		//rect x rect collision
		return (other->rect->left < rect->left + rect->Width()) && (other->rect->left + other->rect->Width() > rect->left)
			&& (other->rect->top < rect->top + rect->Height()) && (other->rect->top + other->rect->Height() > rect->top);

		break;
	case EColliderType::eCircle:
		//CIRCLE x RECT COLLISION NOT IMPLEMENTED
		break;
	default:
		break;
	}
}

bool Collider::OverlapsCircle(Collider* other)
{
	//check own collider type
	switch (colliderType)
	{
	case EColliderType::eBox:
		//CIRCLE x RECT COLLISION NOT IMPLEMENTED
		break;
	case EColliderType::eCircle:

		//circle x circle collision
		return (other->circ->x - circ->x) * (other->circ->x - circ->x) + (circ->y - other->circ->y) * (circ->y - other->circ->y)
			< (circ->radius + other->circ->radius) * (circ->radius + other->circ->radius);

		break;
	default:
		break;
	}
}

bool Collider::Overlaps(Collider* other)
{
	switch (other->getType())
	{
	case EColliderType::eBox:
		return OverlapsRect(other);
		break;

	case EColliderType::eCircle:
		return OverlapsCircle(other);
		break;

	default:
		std::cout << "Invalid collision?" << std::endl;
		break;
	}
};

void Collider::DebugDraw()
{
	if (colliderType != EColliderType::eBox) return;

	int posX{ rect->left };
	int posY{ rect->top };

	Rectangle screenRect(0, SCR_WIDTH, 0, SCR_HEIGHT);
	Rectangle clippedRect(*rect);

	clippedRect.ClipTo(screenRect);

	if (clippedRect.IsOutsideOf(screenRect)) {
		//std::cout << "OUTSIDE OF SCREEN" << std::endl;
		return;
	}


	int screenOffset = (posX + posY * screenRect.Width()) * 4;

	int endOfLineOffset = (screenRect.Width() - clippedRect.Width()) * 4;

	if (posX < 0)
	{
		posX = posX;
	}

	for (size_t y = 0; y < clippedRect.Height(); y++)
	{
		for (size_t x = 0; x < clippedRect.Width(); x++)
		{
			if (x + posX < 0 || x + posX >= screenRect.Width() || y + posY < 0 || y + posY >= screenRect.Height()) {//change hardcode later

			}
			else
			{




				float ratio = 155 / 255.0f;

				HAPISPACE::BYTE screenR = scrPtr[screenOffset];
				HAPISPACE::BYTE screenG = scrPtr[screenOffset + 1];
				HAPISPACE::BYTE screenB = scrPtr[screenOffset + 2];

				HAPISPACE::BYTE finalR = (HAPISPACE::BYTE)(ratio * 1 + (screenR * (1 - ratio)));
				HAPISPACE::BYTE finalG = (HAPISPACE::BYTE)(ratio * 100 + (screenG * (1 - ratio)));
				HAPISPACE::BYTE finalB = (HAPISPACE::BYTE)(ratio * 100 + (screenB * (1 - ratio)));

				scrPtr[screenOffset] = finalR;
				scrPtr[screenOffset + 1] = finalG;
				scrPtr[screenOffset + 2] = finalB;


			}
			//texture += 4;
			screenOffset += 4;
		}

		screenOffset += endOfLineOffset;
		//texture += endOfTexOffset;
	}


}
