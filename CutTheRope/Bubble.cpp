#include "Bubble.h"
#include <cmath>
#include <iostream>

using namespace sf;
Bubble::Bubble()
{
    velocity = Vector2f(0.f, -50.f);
    texture.loadFromFile("..\\pic\\bubble.png");
    m_Sprite.setTexture(texture);
    m_Sprite.setScale(0.5f, 0.5f);
    radius = m_Sprite.getGlobalBounds().width / 2;
    m_Sprite.setOrigin(radius * 2, radius * 2);

    // Звуковые файлы
    if (!musicInBubble.openFromFile("..\\music\\bubble.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;
    if (!musicOutBubble.openFromFile("..\\music\\bubble_break.wav"))
        std::cerr << "Ошибка загрузки monster_open.wav" << std::endl;

    // Настройка громкости
    musicInBubble.setVolume(50);  // Громкость для звука грусти
    musicOutBubble.setVolume(50); // Громкость для звука открытия пасти
}
void Bubble::to(float startX, float startY)
{
    m_Sprite.setPosition(startX, startY);
    position = Vector2f(startX, startY);
}

template <typename T>
T lerp(T a, T b, T t) {
    return a + t * (b - a);
}

void Bubble::update(float deltaTime) 
{
    if (popped) return;

    float halfPeriod = 1.0f;  // Полупериод в секундах

    // Постоянное движение вверх (базовая скорость)
    Vector2f upwardVelocity(-15.f, -50.f);  // Вертикальная скорость пузыря

    // Текущий воздушный поток
    Vector2f airFlow = calculateAirFlow(position);

    // Если захват конфеты
    if (capturing) 
    {
        if (musicInBubble.getStatus() != Music::Playing && areSoundsEnabled)
        {
            musicInBubble.play();  // Проигрываем звук открытия пасти
        }
        if (capturedOffset < captureDistance) 
        {
            float offset = captureVelocity.y * deltaTime;
            position.y += offset;
            position.x -= offset / 2.5f;
            capturedOffset += offset;
        }
        else 
        {
            capturing = false;
            moving = true;

            // Устанавливаем начальную вертикальную скорость
            velocity = upwardVelocity;
        }
    }
    else if (moving) 
    {
        if (airFlowActive) 
        {
            // Если воздушный поток активен
            velocity += airFlow * deltaTime; // Добавляем влияние потока
            timeInAirFlow += deltaTime;      // Увеличиваем время в потоке
        }
        else if (exitingAirFlow) 
        {
            // Если поток выключен, но сохраняем инерцию
            velocity.x *= 0.9999f;
            if (std::abs(velocity.x) < 0.001f) 
            {
                // Прекращаем учитывать инерцию, когда скорость становится мала
                exitingAirFlow = false;
            }
        }

        // Постоянное движение вверх
        velocity.y = std::min(velocity.y, upwardVelocity.y);
        position += velocity * deltaTime;

        // Сопротивление (затухание скорости)
        velocity *= 0.999f;

        // Сброс времени в потоке и начало инерционного движения, если поток выключен
        if (!airFlowActive && timeInAirFlow > 0.f) {
            timeInAirFlow = 0.f; // Сбрасываем время в потоке
            exitingAirFlow = true; // Начинаем учитывать инерцию
        }
    }

    // Обновляем положение спрайта
    to(position.x, position.y);
}


void Bubble::draw(RenderWindow& window)
{
    if (!popped) 
    {
        window.draw(m_Sprite);
    }
}

bool Bubble::containsCandy(Vector2f candyPosition)
{
    if (popped) return false;

    Vector2f bubbleCenter = m_Sprite.getPosition();
    float distance = std::sqrt(
        std::pow(candyPosition.x - bubbleCenter.x, 2) +
        std::pow(candyPosition.y - bubbleCenter.y, 2)
    );

    return distance <= radius / 2.f;
}


void Bubble::startMoving()
{
    moving = true; // Пузырь начинает двигаться
}

void Bubble::pop()
{
    popped = true; // Пузырь исчезает
    moving = false;
    if (musicOutBubble.getStatus() != Music::Playing && areSoundsEnabled)
    {
        musicOutBubble.play();  // Проигрываем звук открытия пасти
    }
}

Sprite Bubble::getSprite()
{
    return m_Sprite;
}

Vector2f Bubble::getPosition()
{
    return position;
}

float Bubble::getRadius()
{
    return radius;
}

Vector2f Bubble::calculateAirFlow(Vector2f position)
{
    // Параметры кривой
    float A = 300.f;  // Амплитуда
    float B = 0.005f;  // Частота
    float C = 0.f;    // Смещение
    float D = 700.f;  // Вертикальное смещение
    float delta = 100; // Ширина окрестности
    float airFlowStrength = 500.f; // Сила потока

    float yCurve = A * std::sin(B * position.x + C) + D;

    // Проверяем, находится ли пузырь в окрестности кривой
    if (std::abs(position.y - yCurve) <= delta) 
    {
        // Касательное направление к кривой
        float derivative = A * B * std::cos(B * position.x + C); // Производная y' = A*B*cos(Bx+C)
        Vector2f direction(1.f, derivative); // Касательный вектор
        float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        // Возвращаем нормализованный касательный вектор с заданной силой
        return -(direction / magnitude) * airFlowStrength;
    }

    // Если пузырь далеко от кривой, возвращаем нулевой поток
    return Vector2f(0.f, 0.f);

}

void Bubble::setAirFlowActive(bool active) {
    airFlowActive = active;

    if (!active) {
        velocity += airFlowVelocity;
        airFlowVelocity = { 0.f, 0.f }; // Обнуляем скорость потока
    }

}

void Bubble::setAirFlowDirection(const sf::Vector2f& direction, float strength) {
    if (airFlowActive) {
        // Устанавливаем скорость потока воздуха
        airFlowVelocity = direction * strength;
    }
}
