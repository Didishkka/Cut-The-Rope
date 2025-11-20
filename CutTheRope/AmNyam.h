#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

class AmNyam
{
private:

    Sprite m_Sprite;
    Texture m_Texture;

    Vector2f spriteSize;

    float elapsedTime = 0.f;
    float timeSinceLastHungryAnimation = 0.f;

    bool isHungry = false;
    Vector2f candyPos;
    bool isCandyInRectangle();
    bool isCandyOutOfScene();
    bool candyIsEaten = false;

    // Звуки
    Music musicSad;
    Music musicOpen;
    bool areSoundsEnabled = true;

public:

    AmNyam();

    //~AmNyam();

    Sprite getSprite();

    void animation(float deltaTime);
    
    void draw(float deltaTime, RenderWindow& m_Window);
    
    void setCandyPos(float x, float y);

    Vector2f getSize();
    
    void toggleSounds(bool sounds) { areSoundsEnabled = sounds; }
};