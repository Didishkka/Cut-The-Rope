#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

class Bubble {
public:
    Bubble();
    void to(float startX, float startY);
    void update(float deltaTime);
    void draw(RenderWindow& window);
    bool containsCandy(Vector2f candyPosition);
    void startMoving();  // Начать движение вверх
    void pop();  // "Лопнуть" пузырь
    Vector2f getPosition();
    void captureCandy() { capturing = true; }  // Начать захват
    float getRadius();
    
    bool isCapturing() const { return capturing; } // Проверка, захватывает ли
    bool isPopped() const { return popped; }
    bool isMoving() const { return moving; }
    Sprite getSprite();
    Vector2f captureVelocity = { 0.f, 100.f }; // Скорость опускания пузыря
    Vector2f calculateAirFlow(Vector2f position);
    void setAirFlowActive(bool active); // Установить состояние потока воздуха
    void setAirFlowDirection(const sf::Vector2f& direction, float strength);




private:
    Texture texture;
    Sprite m_Sprite;
    float radius;
    bool popped = false;
    bool moving = false;  // Флаг движения
    Vector2f velocity; // Скорость пузыря для плавания вверх
    Vector2f position;
    bool capturing = false;  // Пузырь захватывает конфету
    float captureTime = 0.f; // Время захвата
    const float captureDelay = 1.0f; // Задержка перед подъёмом
    float captureDistance = 80.f; // Максимальное расстояние для опускания
    float capturedOffset = 0.f;   // Текущее смещение вниз во время захвата
    bool airFlowActive = false; // Флаг активности потока воздуха
    sf::Vector2f airFlowVelocity = { 0.f, 0.f }; // Скорость потока воздуха
    float timeInAirFlow = 0.f; // Время в зоне воздушного потока (в секундах)
    bool exitingAirFlow = false; // Флаг выхода из воздушного потока

    // Звуки
    Music musicInBubble;
    Music musicOutBubble;
    bool areSoundsEnabled = true;

public:
    void toggleSounds(bool sounds) { areSoundsEnabled = sounds; }
};