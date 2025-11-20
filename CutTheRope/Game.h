#pragma once

#include <SFML/Graphics.hpp>
#include "BaseScene.h"
#include "Start.h"
#include "Loading.h"
#include "Level.h"
#include "Settings.h"
#include "LevelMap.h"

using namespace sf;

class Game
{
private:
    std::unique_ptr<BaseScene> currentScene;
    RenderWindow m_Window;
    std::map<int, int> levelStars; // Номер уровня -> количество звезд
    Music backgroundMusic; // Объект для фоновой музыки
    std::string currentMusicFile;  // Хранит текущий файл музыки
    bool isMusicEnabled = true; // Флаг: включена ли музыка
    bool areSoundsEnabled = true; // Флаг: включены ли звуки

    void toggleMusic(bool enable); // Метод включения/выключения музыки
    void toggleSounds(bool enable); // Метод включения/выключения звуков
    void playMusicForScene(int scene); // Метод для управления музыкой



public:
    // Конструктор движка
    Game();

    // Функция старт вызовет все приватные функции
    void start();
    void changeCurrentScene(int newScene);
    void input(Event& event, float deltaTime, RenderWindow& m_Window);
};