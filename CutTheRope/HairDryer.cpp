#include "HairDryer.h"
#include <iostream>

HairDryer::HairDryer() 
{
    texture.loadFromFile("..\\pic\\dyson.png");
    size = Vector2f(500.f, 500.f);
    sprite.setTextureRect(IntRect(0, 0, size.x, size.y));
    sprite.setTexture(texture);
    sprite.setScale(0.95f, 0.95f);
    sprite.setRotation(30);

    // Звуковые файлы
    if (!music.openFromFile("..\\music\\zvuk-sushki-fenom.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;

    // Настройка громкости
    music.setVolume(15);
}

void HairDryer::to(float x, float y) 
{
    sprite.setPosition(x, y);
}

void HairDryer::input(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

        if (sprite.getGlobalBounds().contains(mousePos)) 
        {
            airFlowActive = !airFlowActive; // Переключаем состояние потока
            sprite.setTextureRect(IntRect(0, airFlowActive * size.y, size.x, size.y));

            if (music.getStatus() != Music::Playing && areSoundsEnabled && airFlowActive)
            {
                music.play();  // Проигрываем звук открытия пасти
            }
            else if (!airFlowActive && music.getStatus() == Music::Playing)
            {
                music.stop();
            }
        }
    }
}

void HairDryer::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

bool HairDryer::isAirFlowActive() const {
    return airFlowActive;
}
