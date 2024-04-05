#include "Ent_Background.h"

void Ent_Background::AddNewLayer(std::string gfxName, Vector2 speed, eScrollType scrollType, eScrollDirection direction)
{
    graphics.push_back(gfxName);
    positions.push_back({ 0,0 });
    scrollSpeeds.push_back(speed);
    scrollTypes.push_back(scrollType);
    scrollDirections.push_back(direction);
}

void Ent_Background::Update(float dt)
{
    //eventually add rollback to prevent pos from getting too high
    for (size_t i = 0; i < graphics.size(); i++)
    {
        positions[i] = positions[i] + scrollSpeeds[i] * dt;
    }
}
void Ent_Background::Render(IGraphics& viz)
{
    //viz.DrawSprite(m_gfxName, m_posX, m_posY, true, eScrollType::HorizontalAndVertical, eScrollDirection::HorizFOR_Vert_FOR);
    //viz.DrawSprite(m_gfxName, m_posX, m_posY, true, eScrollType::Vertical, eScrollDirection::Forward);
    for (size_t i = 0; i < graphics.size(); i++)
    {
        viz.DrawSprite(graphics[i], positions[i].x, positions[i].y, true, scrollTypes[i], scrollDirections[i]);
    }

}
;
