#define _USE_MATH_DEFINES
#include "Candy.h"
#include <iostream>
#include <cmath>

auto desktopMode = VideoMode::getDesktopMode();
float screenWidth = static_cast<float>(desktopMode.width);
float screenHeight = static_cast<float>(desktopMode.height);

Candy::Candy()
{
    m_Texture.loadFromFile("..\\pic\\candy.PNG");
    m_Texture.setSmooth(true);
    m_Sprite.setTexture(m_Texture);
    m_Sprite.setScale(0.5f, 0.5f);
    velocity = { 0.f, 0.f };
    attachedToRope = true;  // Конфета изначально привязана к веревке
    candyRadius = m_Sprite.getGlobalBounds().width / 2.f;
    m_Sprite.setOrigin(candyRadius * 2, candyRadius * 2);
}

void Candy::to(float startX, float startY)
{
    m_Sprite.setPosition(startX, startY);
    position = Vector2f(startX, startY);
}

void Candy::setAmNyamSize(Vector2f spriteSize)
{
    AmNyamSize = spriteSize;
}

bool Candy::candyIsFallen()
{
    if (isEatten)
        return isEatten;

    RectangleShape r(AmNyamSize);
    r.setPosition((VideoMode::getDesktopMode().width - AmNyamSize.x) / 2,
        VideoMode::getDesktopMode().height - AmNyamSize.y / 1.4f);
    if (r.getGlobalBounds().contains(position))
    {
        isEatten = true;
        isFallen = true;
    }

    if (position.x > VideoMode::getDesktopMode().width || position.x < -candyRadius ||
        position.y > VideoMode::getDesktopMode().height || position.y < -candyRadius)
    {
        isFallen = true;
    }

    return isFallen;
}

void Candy::checkCollisionWithBoard(float deltaTime)
{
    if (attachedToRope)
        return;

    float boardAngleRad = barrier.getRotation() * M_PI / 180.f;

    // Размеры спрайта (локальные)
    sf::FloatRect localBounds = barrier.getLocalBounds();

    // Координаты углов доски в мировых координатах
    sf::Vector2f topLeft = Vector2f(barrier.getPosition().x, barrier.getPosition().y);
    sf::Vector2f topRight = Vector2f(topLeft.x + barrier.getGlobalBounds().width, 
        topLeft.y - barrier.getGlobalBounds().height * (boardAngleRad < 1 ? -1 : 1) - 35 * (boardAngleRad > 1 ? -1 : 1));

    // Вычисляем параметры линии доски
    float x1 = topLeft.x;
    float y1 = topLeft.y;
    float x2 = topRight.x;
    float y2 = topRight.y;

    // Центр конфеты
    sf::Vector2f candyCenter(position.x, position.y);

    // Параметры уравнения линии Ax + By + C = 0
    float A = y2 - y1;
    float B = x1 - x2;
    float C = x2 * y1 - x1 * y2;

    // Расстояние от центра конфеты до линии доски
    float distance = std::abs(A * candyCenter.x + B * candyCenter.y + C) / std::sqrt(A * A + B * B);

    // Проверяем проекцию конфеты на линию доски
    float dotProduct = (candyCenter.y - y1) * A - (candyCenter.x - x1) * B;

    // Проверка столкновения
    if (dotProduct >= 0 && distance <= candyRadius &&
        candyCenter.y + candyRadius <= std::max(y1, y2))
    {
        time_b += deltaTime;
        isSliding = true;

        // Проекция на линию доски
        float slope = std::sin(boardAngleRad) / std::cos(boardAngleRad);
        float yProjected = y1 + slope * (position.x - x1);

        // Ограничиваем перемещение
        if (position.y > yProjected - candyRadius) 
        {
            position.y = yProjected - candyRadius;
        }

        // Ускорение вдоль наклонной доски
        float acceleration = Rope::GRAVITY * std::sin(boardAngleRad);

        // Обновляем скорости
        velocity.x = acceleration * time_b * std::cos(boardAngleRad);
        velocity.y = acceleration * time_b * std::sin(boardAngleRad);

        // === Вращение конфеты ===
        const float rotationFactor = 900; // Коэффициент для ускорения вращения
        float linearSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        float angularSpeed = rotationFactor * (linearSpeed / candyRadius);
        angle += angularSpeed * deltaTime * (velocity.x > 0 ? 1.f : -1.f);
        float angleInDegrees = angle * 180 / M_PI;
        m_Sprite.setRotation(angleInDegrees);
    }
    else
    {
        isSliding = false;
    }
}

void Candy::checkCollisionWithTrampoline(float deltaTime)
{
    if (attachedToRope)
        return;

    trampoline.setPosition(screenWidth / 3.8f + 46, 
        screenHeight - trampoline.getLocalBounds().height * 0.55f * 0.7f);
    trampoline.setScale(0.25, 0.35);
    FloatRect t = trampoline.getGlobalBounds();
    if (m_Sprite.getGlobalBounds().intersects(t)) 
    {
        time_tr += deltaTime;
        // Устанавливаем начальную скорость и угол выброса
        float launchSpeed = 225.f; // Примерная скорость
        float launchAngle = -M_PI / 2.4f; // Угол в радианах (-45 градусов)

        velocity.x = launchSpeed * std::cos(launchAngle) * time_tr;
        velocity.y = launchSpeed * std::sin(launchAngle) * time_tr;

        // Активируем движение
        inAir = true;
    }
    
}


void Candy::applyPhysics(float deltaTime)
{
    if (attachedToRope)
        return;
    
    if (!isSliding)
        velocity.y += Rope::GRAVITY * deltaTime;

    position.y += velocity.y;
    position.x += velocity.x;
}


void Candy::update(float deltaTime, const Rope& rope, Bubble& bubble)
{
    if (attachedToRope)
    {
        const auto& lastNode = rope.getLastNode();
        Vector2f ropePosition = lastNode.position;

        Vector2f directionToRope = ropePosition - position;
        float distanceToRope = std::sqrt(directionToRope.x * directionToRope.x + directionToRope.y * directionToRope.y);

        if (distanceToRope > Rope::SEGMENT_LENGTH)
        {
            directionToRope /= distanceToRope;
            position += directionToRope * (distanceToRope - Rope::SEGMENT_LENGTH);
        }

        if (rope.cutOccurred)
        {
            attachedToRope = false;
        }
    }

    if (bubble.containsCandy(position))
    {
        if (!bubble.isCapturing() && !bubble.isMoving())
        {
            position = bubble.getPosition();
            bubble.captureCandy(); // Запускаем стадию захвата
        }

        if (bubble.isCapturing())
        {
            // Конфета опускается вместе с пузырём
            position = bubble.getPosition();
        }
        else if (bubble.isMoving())
        {
            // Конфета движется вверх вместе с пузырём
            position = bubble.getPosition();

            velocity = { 0.f, 0.f };
        }

    }
    else
    {
        applyPhysics(deltaTime); // Если конфета не в пузыре, применяем физику
    }


    checkCollisionWithBoard(deltaTime);
    checkCollisionWithTrampoline(deltaTime);
    to(position.x, position.y);

    if (isFallen || stopTime != 0.f)
    {
        isFallen = false;
        stopTime += deltaTime;
        if (stopTime > 2.f)
            isFallen = true;
    }
}




void Candy::draw(RenderWindow& m_Window)
{
    if (!candyIsFallen())
    {
        m_Window.draw(m_Sprite);
    }
}

Vector2f Candy::getPosition()
{
    return position;
}

Sprite Candy::getSprite()
{
    return m_Sprite;
}

void Candy::setBarrier(Sprite sprite)
{
    barrier = sprite;
}

void Candy::setTrampoline(Sprite sprite)
{
    trampoline = sprite;
}

float Candy::getRadius()
{
    return candyRadius;
}

bool Candy::fallen()
{
    return isFallen;
}
bool Candy::eatten()
{
    return isEatten;
}