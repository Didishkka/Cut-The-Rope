#pragma once

#include "BaseScene.h"

using namespace sf;

class Loading : public BaseScene
{
private:

    Sprite m_LoadingSprite;
    Texture m_LoadingTexture;

    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;

    float elapsedTime = 0.0f;
    bool isCycle = false;
public:

    Loading();
    void animation(float deltaTime);
    void draw(float deltaTime, RenderWindow& m_Window) override;
    void input(Event& event, float deltaTime, RenderWindow& m_Window) {};
    int getCurrentScene() override;
    int getNewScene() override;
};