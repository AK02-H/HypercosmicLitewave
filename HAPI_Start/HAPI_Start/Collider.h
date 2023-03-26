#pragma once
#include "NGLOBALS.h"
#include <HAPI_lib.h>
#include "Rectangle.h"



enum class EColliderType
{
	eBox,
	eCircle
};

class Collider
{
private:


	EColliderType colliderType;

	bool OverlapsRect(Collider* other);

	bool OverlapsCircle(Collider* other);
	
public:

	bool isActive{ true };

	Rectangle* rect;
	Circle* circ;

	HAPISPACE::BYTE* scrPtr;


	//Initialises as rectangle collider
	Collider(Rectangle* rect)
	{
		colliderType = EColliderType::eBox;
		this->rect = rect;

		if (DEBUG_COLLIDERS) scrPtr = HAPI.GetScreenPointer();

	}

	//Initialises as circle collider
	Collider(Circle* circ)
	{
		colliderType = EColliderType::eCircle;
		this->circ = circ;

		if (DEBUG_COLLIDERS) scrPtr = HAPI.GetScreenPointer();
	}

	const EColliderType getType() { return colliderType; }


	//Changes to rectangle if not already one?
	void setRect(Rectangle* rect)
	{
		colliderType = EColliderType::eBox;
		this->rect = new Rectangle(*rect);
	};


	virtual bool Overlaps(Collider* other);

	void DebugDraw();
	
};




