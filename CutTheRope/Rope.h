#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>


using namespace sf;

class Rope
{
private:

    static const int NUM_SEGMENTS;        // Количество сегментов
    static const float SEGMENT_LENGTH; // Длина каждого сегмента
    static const float ROPE_THICKNESS;
    static const float GRAVITY;         // Сила гравитации для падающей части
    static const float DAMPING;        // Коэффициент затухания
    static const float FADE_SPEED;      // Скорость исчезновения оставшейся части
    static const int WINDOW_WIDTH;        // Ширина окна
    static const int WINDOW_HEIGHT;       // Высота окна

    struct Node
    {
        Vector2f position;
        Vector2f old_position;
        bool isCut;  // Флаг для отслеживания отрезанных узлов
        float alpha; // Уровень прозрачности для исчезновения

        Node(float x, float y);
        void applyGravity(float deltaTime);
        void verletIntegration(float deltaTime);
        void updateFade();
    };

    bool cutOccurred = false;        // Флаг для отслеживания, произошло ли отрезание

    void constrain(Node& node1, Node& node2);
    void cutRope(RenderWindow& m_Window);
    std::vector<Node> nodes;
    size_t cutIndex = NUM_SEGMENTS; // Индекс отрезанного узла

    bool isMouseButtonPressed = false; // Флаг для отслеживания, нажата ли мышка

    int levelId = 0;

    Music music;
    bool areSoundsEnabled = true;

public:

    Rope() = default;
    void setLevelId(int id);
    void animation(float deltaTime);
    void draw(float deltaTime, RenderWindow& m_Window);
    void input(Event& event, float deltaTime, RenderWindow& m_Window);

    const Node& getLastNode() const;
    void toggleSounds(bool sounds) { areSoundsEnabled = sounds; }

    friend class Candy;
};