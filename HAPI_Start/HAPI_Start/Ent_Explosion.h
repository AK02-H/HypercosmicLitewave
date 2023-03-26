#pragma once
#include "Entity.h"
#include "Animator.h"
class Ent_Explosion :
    public Entity
{
private:

    Animator animator;
    SpriteEffect spriteEffect = SpriteEffect::eNone;

public:

    Ent_Explosion(std::string gfxName, int frameRate) //inherited constructor smells
    {
        m_gfxName = gfxName;
        animator = Animator(gfxName, frameRate);
        animator.shouldLoop = false;
    }

    void Load() override final {};

    void Update(float dt) override final;

    void Render(IGraphics& viz) override final;

    void SetSpriteEffect(SpriteEffect newEffect) { spriteEffect = newEffect; }

    void LateUpdate(float dt) override final {};

    void SetActive(bool active) override final
    {
        isActive = active;
        if (isActive)
        {
            animator.frame = 0;
        }

        spriteEffect = SpriteEffect::eNone;
    };

    Animator* GetAnimator()
    {
        return &animator;
    }
};

