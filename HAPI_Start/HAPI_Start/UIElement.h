#pragma once
#include "IGraphics.h"
#include "Vector2.h"
#include "Rectangle.h"

class UIElement
{
protected:

	std::string m_gfxName = "";
	int m_posX{ 0 };
	int m_posY{ 0 };
	Rectangle boundingBox{0, 0, 0, 0};
	UIElement* m_parent{ nullptr };

public:	//this class is no longer abstract

	bool isActive = true;
	SpriteEffect currentSpriteEffectDefault = SpriteEffect::eNone;

	UIElement() {};

	UIElement(std::string graphic)
	{
		m_gfxName = graphic;
	}

	virtual void Update(float dt);
	
	virtual void Render(IGraphics& viz);

	virtual void SetPosition(int x, int y)
	{
		m_posX = x;
		m_posY = y;
	}

	void SetBoundingRect(Rectangle rect)
	{
		boundingBox = rect;
	}

	Rectangle GetRect() { return boundingBox; }

	virtual void SetLocalPosition(int x, int y);

	Vector2 GetLocalCoords(bool excludeThis = false);
	
	UIElement* GetParent() const	//? works without breaking?
	{
		return m_parent;
	}

	bool isHierarchilyActive() const;
	
	void ParentObject(UIElement* newParent);
	
	void Translate(int x, int y)
	{
		m_posX += x;
		m_posY += y;
	}

};

