#pragma once

#include "BaseScene.h"

using namespace sf;

class Start : public BaseScene
{
private:

    // Позиция Конфеты
    Vector2f m_Position;

    // Объявляем спрайт и текстуру для фона
    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;

    // Объявляем спрайт и текстуру для доски под АмНямом на сцене с игрой
    Sprite m_ButPlay_Sprite;
    Texture m_ButPlay_Texture;

    Sprite m_ButExit_Sprite;
    Texture m_ButExit_Texture;

    Sprite m_ButSet_Sprite;
    Texture m_ButSet_Texture;
public:

    Start();
    void input(Event& event, float deltaTime, RenderWindow& m_Window) override;
    void draw(float deltaTime, RenderWindow& m_Window) override;
    int getCurrentScene() override;
    int getNewScene() override;
};