#pragma once

#include <SFML/Graphics.hpp>
#include "Rope.h" // Для взаимодействия с веревкой
#include "Bubble.h" // Для взаимодействия с пузырем

using namespace sf;

class Candy
{
public:
    // Конструктор, инициализирует конфету с заданной позицией
    Candy();

    // Применяет физику к конфете (гравитация, сопротивление, обновление позиции)
    void applyPhysics(float deltaTime);

    // Обновляет состояние конфеты, включая физику и привязку к веревке
    void update(float deltaTime, const Rope& rope, Bubble& bubble);

    // Отображает конфету на экране
    void draw(RenderWindow& m_Window);

    void to(float startX, float startY);
    void setAmNyamSize(Vector2f spriteSize);
    Vector2f getPosition();
    Sprite getSprite();
    float getRadius();
    
    void setBarrier(Sprite sprite);
    void setTrampoline(Sprite sprite);

    bool fallen();
    bool eatten();

private:
    Texture m_Texture;          // Текстура конфеты
    Sprite m_Sprite;            // Спрайт конфеты
    Vector2f position;              // Позиция конфеты
    Vector2f velocity;              // Скорость конфеты
    Vector2f AmNyamSize;
    float stopTime = 0.f;
    Sprite barrier;
    Sprite trampoline;
    void checkCollisionWithBoard(float deltaTime);
    void checkCollisionWithTrampoline(float deltaTime);
    bool candyIsFallen();
    float vel2 = 0;
    float angle = 0;
    float candyRadius = 0.f;
    float time_b = 0; // Время пройденное после взаимодействия с доской
    float time_tr = 0; // Время пройденное после взаимодействия с батутом

    bool attachedToRope = true; // Прикреплена ли конфета к веревке
    bool inBubble = false;
    bool isEatten = false;
    bool isFallen = false;
    bool isSliding = false; // Флаг состояния скольжения
    bool inAir = false;
};

