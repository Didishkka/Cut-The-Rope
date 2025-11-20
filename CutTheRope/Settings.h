#pragma once

#include <SFML/Graphics.hpp>
#include "BaseScene.h"

using namespace sf;

class Settings : public BaseScene
{
private:

    float screenHeight, screenWidth;
    bool musicEnabled; // Локальная копия состояния музыки
    bool soundsEnabled; // Локальная копия состояния звуков

    Text musicToggleText;
    Text soundsToggleText;

    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;

    Sprite m_ButBack_Sprite;
    Texture m_ButBack_Texture;

    Sprite m_Bool1Sprite;
    Sprite m_Bool2Sprite;
    Texture m_BoolTexture;
    Vector2f boolSize;

    Font font;

public:

    Settings(bool musicEnabled, bool soundsEnabled);
    void draw(float deltaTime, RenderWindow& m_Window) override;
    void input(Event& event, float deltaTime, RenderWindow& m_Window);
    int getCurrentScene() override;
    int getNewScene() override;
    bool isMusicToggled() const { return musicEnabled; }
    bool areSoundsToggled() const { return soundsEnabled; }
};