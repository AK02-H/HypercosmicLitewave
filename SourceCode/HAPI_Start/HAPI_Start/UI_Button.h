#pragma once
#include "UIElement.h"
#include "Animator.h"

class UI_Button : public UIElement
{
private:

    const HAPISPACE::HAPI_TMouseData* m_mouseData;
     //rect is only the size of the box,
    //during mouse checks the position is additionally incorporated

    Animator animator;

    bool hoveredOver = false;
    bool mouseWasDownLastFrame = false;

    const float k_holdButtonCallInterval = 0.18f;
    float timer_holdInterval = 0;
    const int k_callsBeforeSpeedUp = 5;
    int counter_calls = 0;

    void PlayButtonSound();
    
public:

    bool allowHold = false;
    bool isPressed = false;
    int commandId = 1;
    bool doNotHoverHighlight = false;
    bool doAnimate = false;

    int* sfxVolume = nullptr;

    UI_Button(std::string graphic) : UIElement(graphic)
    {
        m_gfxName = graphic;
        animator = Animator(m_gfxName, 11);
        animator.shouldLoop = true;
        //animator.EmergencySetAnimName("1Player");
    }


    //KNOWN ERROR, SOMETIMES HOVERING OVER BUTTON AFTER RECENTLY CLICKING
    //ANOTHER BUTTON CAUSES THAT BUTTON EVENT TO FIRE
    void Update(float dt) override;
    
    void Render(IGraphics& viz) override;

    void SetMouseData(const HAPISPACE::HAPI_TMouseData* data) { m_mouseData = data; }

    Animator* GetAnimator() { return &animator; }

    void SetAnimation(IGraphics& viz, std::string animName)
    {
        animator.SetAnimation(viz, animName);
    }

    void EmergencyReset()
    {
        isPressed = false;
        mouseWasDownLastFrame = false;
    }

};

