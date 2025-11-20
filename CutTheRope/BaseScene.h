#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>

using namespace sf;

class BaseScene
{
protected:

    // Объявляем распознание сцены
    std::string scene;

    // Объявляем пару ключ-значение для обозначения текущих сцен
    std::map <std::string, int> mapping;

    int newSceneId; //следующая сцена
    int oldSceneId; //предыдущая сцена
public:
    // Конструктор сцен
    BaseScene();
    virtual void input(Event &event, float deltaTime, RenderWindow &m_Window) = 0;
    virtual void draw(float deltaTime, RenderWindow &m_Window) = 0;
    virtual int getCurrentScene() = 0;
    virtual int getNewScene() = 0;
};