#include "Ent_Bullet.h"

void Ent_Bullet::Load()
{
	//collisionBox = Rectangle(m_posX, m_posX + dbgSz, m_posY, m_posY + dbgSz);

	//PrimeBullet(m_posX, m_posY, 0.5f, 0.5f);
};

void Ent_Bullet::Update(float dt)
{
	SetPosition(m_posX + (m_velX * dt), m_posY + (m_velY * dt));
	//if (DEBUG_COLLIDERS) collider.DebugDraw();
	
	BoundCheck();

	/*testFrame++;
	if (testFrame > 3) testFrame = 0;*/
	animator.Animate(dt);
}

void Ent_Bullet::Render(IGraphics& viz)
{
	if (animateBullet)
	{
		viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame);
	}
	else
	{
		viz.DrawSprite(m_gfxName, m_posX, m_posY);
	}
}



void Ent_Bullet::LateUpdate(float dt)
{
	if (DEBUG_COLLIDERS) collider.DebugDraw();
}

void Ent_Bullet::SetPosition(float x, float y)
{
	m_posX = x;
	m_posY = y;

	//   collisionBox.t

	int width = collisionBox.Width();
	int height = collisionBox.Height();

	collisionBox.left = (int)m_posX;
	collisionBox.right = (int)m_posX + width;
	collisionBox.top = (int)m_posY;
	collisionBox.bottom = (int)m_posY + height;
}

void Ent_Bullet::SetColliderRect(Rectangle& rect)
{
	collisionBox.left = rect.left;
	collisionBox.right = rect.right;
	collisionBox.top = rect.top;
	collisionBox.bottom = rect.bottom;
}

void Ent_Bullet::PrimeBullet(float posX, float posY, float dirX, float dirY, float speed)
{
	SetPosition(posX, posY);
	m_speed = speed;
	m_velX = dirX * m_speed;
	m_velY = dirY * m_speed;

	animator.SetAnimationFrame(0);
}



