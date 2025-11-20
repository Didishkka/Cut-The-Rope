#include "Rope.h"
#include <cmath>
#include <iostream>

void Rope::setLevelId(int id)
{
    levelId = id;
    // Создаем узлы
    for (int i = 0; i < NUM_SEGMENTS; ++i)
    {
        switch (levelId)
        {
        case 1:
            nodes.emplace_back(WINDOW_WIDTH / 2, i * SEGMENT_LENGTH);
            break;
        case 2:
            nodes.emplace_back(WINDOW_WIDTH / 7, i * SEGMENT_LENGTH);            
            break;
        case 3:
            nodes.emplace_back(WINDOW_WIDTH * 5 / 6, i * SEGMENT_LENGTH);
            break;
        default:
            break;
        }
    }

    // Звуковые файлы
    if (!music.openFromFile("..\\music\\rope_get.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;

    // Настройка громкости
    music.setVolume(50);
}

void Rope::animation(float deltaTime)
{
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        if (nodes[i].isCut)
        {
            nodes[i].applyGravity(deltaTime); // Применение гравитации
            nodes[i].verletIntegration(deltaTime); // Интеграция с использованием физики
        }
    }

    // Соглашение для связывания узлов и констрейнтов, если нужно
    for (size_t i = 1; i < nodes.size(); ++i)
    {
        constrain(nodes[i - 1], nodes[i]);
    }
}

void Rope::draw(float deltaTime, RenderWindow& m_Window)
{
    Color ropeColor(123, 63, 0);  // Коричневый цвет веревки

    for (size_t i = 0; i < nodes.size() - 1; ++i)
    {
        // Определяем начальные точки сегмента
        Vector2f start(nodes[i].position.x, nodes[i].position.y);
        Vector2f end;

        // Проверяем, находится ли следующий узел в отрезанной части
        if (i + 1 < nodes.size() && !nodes[i + 1].isCut)
        {
            // Если узел не отрезан, используем его для расчета конца сегмента
            end = Vector2f(nodes[i + 1].position.x, nodes[i + 1].position.y);
        }
        else
        {
            // Если узел отрезан, используем фиксированную длину сегмента
            end = Vector2f(nodes[i].position.x, nodes[i].position.y + SEGMENT_LENGTH);
        }

        // Вычисляем длину и угол сегмента
        Vector2f direction = end - start; 
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length; // Нормализуем направление
        float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265f;

        // Создаем сегмент веревки
        RectangleShape ropeSegment(Vector2f(length, ROPE_THICKNESS));
        ropeSegment.setOrigin(ROPE_THICKNESS / 2, SEGMENT_LENGTH);
        ropeSegment.setPosition(start);
        ropeSegment.setRotation(angle);

        // Отрисовка сегмента с учетом прозрачности
        Color fadedColor_rope = ropeColor;
        fadedColor_rope.a = static_cast<Uint8>(nodes[i].alpha);
        ropeSegment.setFillColor(fadedColor_rope);

        if (nodes[i].alpha > 0) // Отрисовываем только если сегмент видим
        {
            m_Window.draw(ropeSegment);
        }

        // Обновляем состояние узлов после разреза
        if (nodes[i].isCut)
        {
            nodes[i].applyGravity(deltaTime);
            nodes[i].verletIntegration(deltaTime);
            nodes[i].updateFade();
        }
        else if (cutOccurred)
        {
            nodes[i].updateFade();
        }
    }
}



void Rope::cutRope(RenderWindow& m_Window)
{
    if (cutOccurred) // Если уже был разрез, не выполняем повторное действие
        return;

    // Получаем текущую позицию мыши
    Vector2i mousePosition = Mouse::getPosition(m_Window);

    // Ищем ближайший узел для разреза
    for (size_t i = 0; i < nodes.size(); ++i)
    {
        // Проверяем, находится ли мышь в пределах заданного радиуса от узла (5 пикселей)
        float distance = std::hypot(mousePosition.x - nodes[i].position.x, mousePosition.y - nodes[i].position.y);
        if (distance < 10.f)
        {
            // Устанавливаем индекс разреза и отмечаем, что разрез произошел
            cutIndex = i;
            cutOccurred = true;
            break;
        }
    }

    // Если разрез не был найден, выходим
    if (!cutOccurred)
        return;

    // Помечаем узлы начиная с точки разреза как отрезанные
    for (size_t i = cutIndex; i < nodes.size(); ++i)
    {
        nodes[i].isCut = true;
        nodes[i].old_position = nodes[i].position; // Запоминаем текущее положение для последующей физики
    }

    if (music.getStatus() != Music::Playing && areSoundsEnabled)
    {
        music.play();
    }
}

const Rope::Node& Rope::getLastNode() const
{
    return Rope::Node(nodes.back().position.x, nodes.back().position.y);
}


void Rope::input(Event& event, float deltaTime, RenderWindow& m_Window)
{
    switch (event.type)
    {
    case Event::MouseButtonPressed:
        isMouseButtonPressed = true;
        cutRope(m_Window);
        break;
    case Event::MouseMoved:
        if (isMouseButtonPressed)
            cutRope(m_Window);
        break;
    case Event::MouseButtonReleased:
        cutRope(m_Window);
        isMouseButtonPressed = false;
        break;
    }
}

const int Rope::NUM_SEGMENTS = 25;
const float Rope::SEGMENT_LENGTH = 10.0f;
const float Rope::ROPE_THICKNESS = 20.f;
const float Rope::GRAVITY = 9.8f;
const float Rope::DAMPING = 0.98f;
const float Rope::FADE_SPEED = 2.f;
const int Rope::WINDOW_WIDTH = VideoMode::getDesktopMode().width;
const int Rope::WINDOW_HEIGHT = VideoMode::getDesktopMode().height;

// Реализация метода Node

Rope::Node::Node(float x, float y) : position(x, y), old_position(x, y), isCut(false), alpha(255) {}

void Rope::Node::applyGravity(float deltaTime)
{
    if (isCut)
    {
        Vector2f gravity(0, GRAVITY * deltaTime);
        position += gravity;
    }
}

void Rope::Node::verletIntegration(float deltaTime)
{
    if (isCut)
    {
        Vector2f temp = position;
        position += (position - old_position) * DAMPING;
        old_position = temp;
    }
}

void Rope::constrain(Node& node1, Node& node2)
{
    Vector2f diff = node2.position - node1.position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float difference = (SEGMENT_LENGTH - dist) / dist;
    Vector2f offset = diff * (difference / 2);

    node1.position = node1.position - offset;
    node2.position += offset;
}

void Rope::Node::updateFade()
{
    // Уменьшаем прозрачность с заданной скоростью
    alpha -= FADE_SPEED;

    if (alpha < 0)
        alpha = 0;
}

