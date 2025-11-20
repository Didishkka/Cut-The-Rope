#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

class HairDryer 
{
public:
    HairDryer();

    void to(float x, float y); // Установка позиции фена
    void input(const sf::Event& event, const sf::RenderWindow& window); // Обработка нажатия
    void draw(sf::RenderWindow& window); // Отображение фена
    bool isAirFlowActive() const; // Проверка, включён ли поток

private:
    Texture texture;
    Sprite sprite;
    bool airFlowActive = false; // Состояние потока воздуха
    Vector2f size;

    // Звуки
    Music music;
    bool areSoundsEnabled = true;

public:
    void toggleSounds(bool sounds) { areSoundsEnabled = sounds; }
};
