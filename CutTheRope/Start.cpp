#include "Start.h"
#include <thread>

Start::Start()
{

    auto desktopMode = VideoMode::getDesktopMode();
    float screenWidth = static_cast<float>(desktopMode.width);
    float screenHeight = static_cast<float>(desktopMode.height);

    scene = "start";
    newSceneId = mapping["start"];

    // Загружаем фон в текстуру
    if (!m_BackgroundTexture.loadFromFile("..\\pic\\background_start.jpg"))
        throw std::runtime_error("Failed to load background texture");

    m_BackgroundSprite.setTexture(m_BackgroundTexture);
    m_BackgroundSprite.setScale(screenWidth / m_BackgroundTexture.getSize().x,
        screenHeight / m_BackgroundTexture.getSize().y);

    // Загружаем кнопку "Играть"
    if (!m_ButPlay_Texture.loadFromFile("..\\pic\\button_play.png"))
        throw std::runtime_error("Failed to load play button texture");

    m_ButPlay_Sprite.setTexture(m_ButPlay_Texture);
    m_ButPlay_Sprite.setScale(screenHeight / m_ButPlay_Sprite.getGlobalBounds().height * 0.23f,
        screenHeight / m_ButPlay_Sprite.getGlobalBounds().height * 0.23f);
    m_ButPlay_Sprite.setPosition(screenWidth / 2.0f - m_ButPlay_Sprite.getGlobalBounds().width * 0.5f, 
        screenHeight / 2.0f + m_ButPlay_Sprite.getGlobalBounds().height * 0.5f);

    // Загружаем кнопку "Выход"
    if (!m_ButExit_Texture.loadFromFile("..\\pic\\button_exit.png"))
        throw std::runtime_error("Failed to load exit button texture");

    m_ButExit_Sprite.setTexture(m_ButExit_Texture);
    m_ButExit_Sprite.setScale(screenHeight / m_ButExit_Sprite.getGlobalBounds().height * 0.15f,
        screenHeight / m_ButExit_Sprite.getGlobalBounds().height * 0.15f);
    m_ButExit_Sprite.setPosition(5, screenHeight - m_ButExit_Sprite.getGlobalBounds().height - 5);

    // Загружаем кнопку "Настройки"
    if (!m_ButSet_Texture.loadFromFile("..\\pic\\button_settings.png"))
        throw std::runtime_error("Failed to load settings button texture");

    m_ButSet_Sprite.setTexture(m_ButSet_Texture);
    m_ButSet_Sprite.setScale(screenHeight / m_ButSet_Sprite.getGlobalBounds().height * 0.13f, 
        screenHeight / m_ButSet_Sprite.getGlobalBounds().height * 0.13f);
    m_ButSet_Sprite.setPosition(screenWidth - m_ButSet_Sprite.getGlobalBounds().width - 5,
        screenHeight - m_ButSet_Sprite.getGlobalBounds().height - 5);
}

void Start::input(Event& event, float deltaTime, RenderWindow& m_Window)
{
    Vector2i mousePos = sf::Mouse::getPosition(m_Window);

    switch (event.type)
    {
    case Event::KeyReleased:
    case Event::KeyPressed:
        if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            m_ButPlay_Sprite.setColor(Color(255, 255, 255, 255));
            newSceneId = mapping["loading"];   //loading
        }
        break;

    case Event::MouseButtonReleased:

        if (m_ButExit_Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_Window.close();
        }
        else if (m_ButPlay_Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_ButPlay_Sprite.setColor(Color(255, 255, 255, 255));
            newSceneId = mapping["loading"];   //loading
        }
        else if (m_ButSet_Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_ButSet_Sprite.setColor(Color(255, 255, 255, 255));
            newSceneId = mapping["settings"];   //settings
        }
        break;

    case Event::MouseMoved:
        // Обработка состояния кнопок
        if (m_ButPlay_Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
            m_ButPlay_Sprite.setColor(Color(255, 255, 255, 200));
        else
            m_ButPlay_Sprite.setColor(Color(255, 255, 255, 255));

        if (m_ButExit_Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
            m_ButExit_Sprite.setColor(Color(255, 255, 255, 200));
        else
            m_ButExit_Sprite.setColor(Color(255, 255, 255, 255));

        if (m_ButSet_Sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
            m_ButSet_Sprite.setColor(Color(255, 255, 255, 200));
        else
            m_ButSet_Sprite.setColor(Color(255, 255, 255, 255));
        break;

    default:
        break;
    }
}

void Start::draw(float deltaTime, RenderWindow& m_Window)
{
    // Отрисовываем фон и кнопки
    m_Window.draw(m_BackgroundSprite);
    m_Window.draw(m_ButPlay_Sprite);
    m_Window.draw(m_ButExit_Sprite);
    m_Window.draw(m_ButSet_Sprite);
}

int Start::getCurrentScene()
{
    return mapping[scene];
}

int Start::getNewScene()
{
    return newSceneId;
}