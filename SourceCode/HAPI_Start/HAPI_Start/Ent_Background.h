#pragma once
#include "Entity.h"

class Ent_Background :
    public Entity
{
    int m_speed{ 1300 };

    std::vector<std::string> graphics;
    std::vector<Vector2> positions;
    std::vector<Vector2> scrollSpeeds;
    std::vector<eScrollType> scrollTypes;
    std::vector<eScrollDirection> scrollDirections;

public:

    Ent_Background()
    {
        //m_gfxName = gfxName;
    }

    void AddNewLayer(std::string gfxName, Vector2 speed, eScrollType scrollType, eScrollDirection direction);

    void Load() override final {};

    void Update(float dt) override final;
    
    //???
    void Render(IGraphics& viz) override final;

    void LateUpdate(float dt) override final {};
};

