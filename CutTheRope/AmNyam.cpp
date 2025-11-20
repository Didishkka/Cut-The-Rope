#include "AmNyam.h"
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>

AmNyam::AmNyam()
{
    // Загружаем текстуру
    m_Texture.loadFromFile("..\\pic\\AllAmNyams.png");
    m_Texture.setSmooth(true);
    m_Sprite.setTexture(m_Texture);

    // Размер спрайта
    spriteSize = Vector2f(250, 270);
    m_Sprite.setTextureRect(IntRect(0, 0, spriteSize.x, spriteSize.y));
    m_Sprite.setPosition((float)(VideoMode::getDesktopMode().width - spriteSize.x) / 2,
        (float)VideoMode::getDesktopMode().height - 1.25f * spriteSize.y);

    // Инициализация переменных для таймера
    timeSinceLastHungryAnimation = 0.0f;
    isHungry = false;

    // Звуковые файлы
    if (!musicSad.openFromFile("..\\music\\monster_sad.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;
    if (!musicOpen.openFromFile("..\\music\\monster_open.wav"))
        std::cerr << "Ошибка загрузки monster_open.wav" << std::endl;

    // Настройка громкости
    musicSad.setVolume(50);  // Громкость для звука грусти
    musicOpen.setVolume(50); // Громкость для звука открытия пасти

}

bool AmNyam::isCandyInRectangle()
{
    RectangleShape r_mouth(spriteSize + Vector2f(0, spriteSize.y * 2));
    RectangleShape r_candy(spriteSize);
    r_candy.setPosition((VideoMode::getDesktopMode().width - spriteSize.x) / 2,
        VideoMode::getDesktopMode().height - spriteSize.y / 1.4f);
    r_mouth.setPosition((VideoMode::getDesktopMode().width - spriteSize.x) / 2,
        VideoMode::getDesktopMode().height - spriteSize.y / 1.4f - 2 * spriteSize.y);

    if (r_candy.getGlobalBounds().contains(candyPos))
        candyIsEaten = true;

    if (r_mouth.getGlobalBounds().contains(candyPos)) 
    {
        if (musicOpen.getStatus() != Music::Playing && areSoundsEnabled) 
        {
            musicOpen.play();  // Проигрываем звук открытия пасти
        }
        return true;
    }

    return false;
}

bool AmNyam::isCandyOutOfScene()
{
    if (candyPos.y > VideoMode::getDesktopMode().height || candyPos.y < 0 ||
        candyPos.x > VideoMode::getDesktopMode().width || candyPos.x < 0)
    {
        if (!candyIsEaten)
        {
            if (musicSad.getStatus() != Music::Playing && areSoundsEnabled) 
            {
                musicSad.play();  // Проигрываем звук грусти
            }
            return true;
        }
    }
    return false;
}


void AmNyam::animation(float deltaTime)
{
    int frameNum = 10;  // Количество кадров анимации
    int hungryFramenum = 5;
    float animationDuration = 3.5f;  // Длительность одного цикла анимации
    float hungryAnimationDuration = 1.5f;  // Длительность одного цикла голодной анимации
    float hungryInterval = 10.0f;  // Интервал для голодной анимации
    Vector2f hungrySpriteSize(253, 270); // Размер спрайта

    if (isCandyOutOfScene())
    {
        m_Sprite.setTextureRect(IntRect(0, 3 * spriteSize.y, spriteSize.x, spriteSize.y));
        return;
    }

    if (isCandyInRectangle())
    {
        elapsedTime = 0.f;
        timeSinceLastHungryAnimation = 0.f;
        isHungry = false;
        m_Sprite.setTextureRect(IntRect(spriteSize.x, 2 * spriteSize.y, spriteSize.x, spriteSize.y));
        return;
    }
    else musicOpen.stop();

    elapsedTime += deltaTime;
    timeSinceLastHungryAnimation += deltaTime;

    int animFrame = static_cast<int>((elapsedTime / animationDuration) * static_cast<float>(frameNum)) % frameNum;
    // Проверка, нужно ли включить голодную анимацию
    if (timeSinceLastHungryAnimation >= hungryInterval && animFrame == 9)
    {
        if (!isHungry)
        {
            isHungry = true;
            m_Sprite.setTextureRect(IntRect(0, spriteSize.y, spriteSize.x, spriteSize.y));
            timeSinceLastHungryAnimation = 0.0f;   // Сбрасываем таймер
            elapsedTime = 0.0f;
        }
    }

    // Если включена голодная анимация, через некоторое время возвращаем обычную текстуру
    if (isHungry && elapsedTime >= hungryAnimationDuration)
    {
        m_Sprite.setTextureRect(IntRect(0, 0, spriteSize.x, spriteSize.y));
        isHungry = false;
        elapsedTime = 0.0f;
    }

    // Вычисляем текущий кадр анимации
    if (isHungry)
    {
        m_Sprite.setTextureRect(IntRect(0, spriteSize.y, spriteSize.x, spriteSize.y));
        animFrame = static_cast<int>((elapsedTime / hungryAnimationDuration) * static_cast<float>(hungryFramenum)) % hungryFramenum;
        m_Sprite.setTextureRect(IntRect(animFrame * hungrySpriteSize.x, spriteSize.y, hungrySpriteSize.x, hungrySpriteSize.y));
    }
    else
    {
        m_Sprite.setTextureRect(IntRect(0, 0, spriteSize.x, spriteSize.y));
        animFrame = static_cast<int>((elapsedTime / animationDuration) * static_cast<float>(frameNum)) % frameNum;
        m_Sprite.setTextureRect(IntRect(animFrame * spriteSize.x, 0, spriteSize.x, spriteSize.y));
    }
}

void AmNyam::draw(float deltaTime, RenderWindow& m_Window)
{
    animation(deltaTime);
    m_Window.draw(m_Sprite);
}

Sprite AmNyam::getSprite()
{
    return m_Sprite;
}

void AmNyam::setCandyPos(float x, float y)
{
    candyPos.x = x;
    candyPos.y = y;
}

Vector2f AmNyam::getSize()
{
    return spriteSize;
}
