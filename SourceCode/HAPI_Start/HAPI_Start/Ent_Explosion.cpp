#include "Ent_Explosion.h"

void Ent_Explosion::Update(float dt)
{
    animator.Animate(dt);

    if (animator.onCompleteSignal)
    {
        animator.onCompleteSignal = false;
        isActive = false;
    }


}

void Ent_Explosion::Render(IGraphics& viz)
{
    viz.DrawSprite(m_gfxName, m_posX, m_posY, animator.GetCurrentAnimationName(), animator.frame, spriteEffect);
};
;
