#pragma once

#include "BaseScene.h"

using namespace sf;

class LevelMap : public BaseScene
{
private:
    float screenWidth, screenHeight;
    bool iCanOpenSecondLevel = false, iCanOpenThirdLevel = false;

    std::map<int, int> levelStars; // Номер уровня -> количество звезд

    // Объявляем спрайт и текстуру для фона
    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;

    Sprite m_BackSprite;
    Texture m_BackTexture;

    Sprite m_OneLevelCardSprite;
    Texture m_OneLevelCardTexture;

    Sprite m_TwoLevelCardSprite;
    Texture m_TwoLevelCardTexture;

    Sprite m_ThreeLevelCardSprite;
    Texture m_ThreeLevelCardTexture;

    Sprite m_StarsSprite;
    Texture m_StarsTexture;
    Vector2f starSize;

    Sprite m_LockSprite;
    Texture m_LockTexture;

public:

    LevelMap(const std::map<int, int>& stars);
    void input(Event& event, float deltaTime, RenderWindow& m_Window) override;
    void draw(float deltaTime, RenderWindow& m_Window) override;
    int getCurrentScene() override;
    int getNewScene() override;
};