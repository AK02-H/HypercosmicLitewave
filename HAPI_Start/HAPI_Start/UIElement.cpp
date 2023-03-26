#include "UIElement.h"

void UIElement::Update(float dt)
{
	//Check if parents are inactive
	int mrgncy = 0;
	UIElement* current = this;
	while (current != nullptr || mrgncy > 500)	//max loop limit, also max parent limit
	{
		if (!current->isActive) return;
		current = current->GetParent();
		mrgncy++;
	}
}

void UIElement::Render(IGraphics& viz)
{
	if (m_gfxName == "") return;


	//Check if parents are inactive
	int mrgncy = 0;
	UIElement* current = this;
	while (current != nullptr || mrgncy > 500)	//max loop limit, also max parent limit
	{
		if (!current->isActive)
		{
			return;
		}
		current = current->GetParent();
		mrgncy++;
	}

	Vector2 localCoords = GetLocalCoords(true);


	//Vector2 newPos = GetLocalCoords();
	viz.DrawSprite(m_gfxName, m_posX + localCoords.x, m_posY + localCoords.y, "", 0, currentSpriteEffectDefault);
}

void UIElement::SetLocalPosition(int x, int y)
{
	/*Vector2 newPos = GetLocalCoords(true);
	m_posX = newPos.x + x;
	m_posY = newPos.y + y;*/

	Vector2 newPos = GetLocalCoords(true);
	m_posX = x;
	m_posY = y;
}

Vector2 UIElement::GetLocalCoords(bool excludeThis)
{

	int newX = m_posX;
	int newY = m_posY;

	if (excludeThis)
	{
		newX = 0;
		newY = 0;
	}

	int mrgncy = 0;
	//have emergency break
	UIElement* current = m_parent;
	while (current != nullptr || mrgncy > 500)	//max loop limit, also max parent limit
	{
		newX += current->m_posX;
		newY += current->m_posY;

		current = current->GetParent();
		mrgncy++;
	}

	return { (float)newX, (float)newY };

}

bool UIElement::isHierarchilyActive() const
{
	//Check if parents are inactive
	int mrgncy = 0;
	const UIElement* current = this;
	bool is = false;

	//loop through all parents, if any of them are inactive then this should be inactive

	while (current != nullptr || mrgncy > 500)
	{
		if (!current->isActive)
		{
			return false;
		}
		current = current->GetParent();
		mrgncy++;
	}

	return true;

}
void UIElement::ParentObject(UIElement* newParent)
{

	//check to make sure there is no loop
	if (newParent != this)
	{
		//make sure parent doesn't exist already
		//need to make sure none of the parents' parents are this
		//and that none of parents' children are children of this?
		//should work itself out

		m_parent = newParent;

	}
	else
	{
		//assert?
		HAPI.UserMessage("Warning: cannot parent UI element to self", "Error");

	}
}
;
