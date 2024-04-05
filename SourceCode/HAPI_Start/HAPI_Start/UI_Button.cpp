#include "UI_Button.h"

void UI_Button::PlayButtonSound()
{
    HAPI_TSoundOptions options;

    if (sfxVolume != nullptr)
    {
        options = HAPI_TSoundOptions((float)*sfxVolume / (float)100, false);
    }
    else
    {
        options = HAPI_TSoundOptions(0.8f, false);
    }


    if (!HAPI.PlaySound("Data/Audio/ui_button.wav", options))
    {
        HAPI.UserMessage("Warning, ui sound not found", "UI Audio error");
    }
}

void UI_Button::Update(float dt)
{
    UIElement::Update(dt);

    if (!isHierarchilyActive()) return;


    if (doAnimate) animator.Animate(dt);

    Rectangle translatedRect(boundingBox);
    Vector2 newPos = GetLocalCoords();
    translatedRect.Translate(newPos.x, newPos.y);

    if (!doNotHoverHighlight)
    {
        if (!hoveredOver)
        {
            if (translatedRect.IfPointWithin(m_mouseData->x, m_mouseData->y))
            {
                hoveredOver = true;
            }
        }
        else
        {
            if (!translatedRect.IfPointWithin(m_mouseData->x, m_mouseData->y))
            {
                hoveredOver = false;
            }
        }
    }


    //std::cout << "mouse x " << m_mouseData->x << " mouse y " << m_mouseData->y << std::endl;
    if (m_mouseData->leftButtonDown)
    {

        if (translatedRect.IfPointWithin(m_mouseData->x, m_mouseData->y))
        {

            if (!allowHold)
            {
                //should prevent multiple calls from mouse being held down
                if (!mouseWasDownLastFrame)
                {
                    isPressed = true;
                    mouseWasDownLastFrame = true;
                    PlayButtonSound();
                    std::cout << "Button pressed at " << HAPI.GetTime() << std::endl;

                }
                else
                {
                    isPressed = false;
                    //mouseWasDownLastFrame = false;
                }
            }
            else
            {

                if (!mouseWasDownLastFrame)
                {
                    isPressed = true;
                    mouseWasDownLastFrame = true;

                    std::cout << "Button pressed at " << HAPI.GetTime() << std::endl;

                }
                else
                {


                    timer_holdInterval += dt;
                    if (timer_holdInterval > (counter_calls < k_callsBeforeSpeedUp ? (k_holdButtonCallInterval) : (k_holdButtonCallInterval / 3)))
                    {
                        mouseWasDownLastFrame = false;
                        counter_calls++;
                        PlayButtonSound();
                        timer_holdInterval = 0;
                    }

                    isPressed = false;

                }
            }


        }
        else
        {
            isPressed = false;
            mouseWasDownLastFrame = false;

            if (allowHold)
            {
                timer_holdInterval = 0;
                counter_calls = 0;
            }
        }

    }
    else
    {
        isPressed = false;
        mouseWasDownLastFrame = false;


        if (allowHold)
        {
            timer_holdInterval = 0;
            counter_calls = 0;
        }

    }
}

void UI_Button::Render(IGraphics& viz)
{
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

    if (!doNotHoverHighlight)
    {
        if (!hoveredOver)
        {
            viz.DrawSprite(m_gfxName, m_posX + localCoords.x, m_posY + localCoords.y, animator.GetCurrentAnimationName());
        }
        else
        {
            viz.DrawSprite(m_gfxName, m_posX + localCoords.x, m_posY + localCoords.y, animator.GetCurrentAnimationName(), 0, SpriteEffect::eNegative);
        }
    }
    else
    {
        viz.DrawSprite(m_gfxName, m_posX + localCoords.x, m_posY + localCoords.y, animator.GetCurrentAnimationName(), 0, currentSpriteEffectDefault);

    }

}
;
