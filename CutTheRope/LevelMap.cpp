#include "LevelMap.h"


LevelMap::LevelMap(const std::map<int, int>& stars) : levelStars(stars)
{
    auto desktopMode = VideoMode::getDesktopMode();
    screenWidth = static_cast<float>(desktopMode.width);
    screenHeight = static_cast<float>(desktopMode.height);

    scene = "level_map";
    newSceneId = mapping["level_map"];
    oldSceneId = mapping["start"];

    for (auto it = levelStars.begin(); it != levelStars.end(); ++it)
    {
        int level = it->first;  // Ключ из map (уровень)
        int stars = it->second; // Значение из map (звезды)
        if (level == 1 && stars == 3)
            iCanOpenSecondLevel = true;
        if (level == 2 && stars == 3)
            iCanOpenThirdLevel = true;
    }

    if (!iCanOpenSecondLevel)
    {
        m_TwoLevelCardSprite.setColor(Color(255, 255, 255, 150));
    }
    else
        m_TwoLevelCardSprite.setColor(Color(255, 255, 255, 255));

    if (!iCanOpenThirdLevel)
    {
        m_ThreeLevelCardSprite.setColor(Color(255, 255, 255, 150));
    }
    else
        m_ThreeLevelCardSprite.setColor(Color(255, 255, 255, 255));

    ///////////////////////////
    // Загружаем фон в текстуру
    m_BackgroundTexture.loadFromFile("..\\pic\\background_simple.png");
    m_BackgroundSprite.setTexture(m_BackgroundTexture);
    m_BackgroundSprite.setScale(screenWidth / m_BackgroundTexture.getSize().x,
        screenHeight / m_BackgroundTexture.getSize().y);

    // Загружаем кнопку для перехода на предыдущую сцену
    m_BackTexture.loadFromFile("..\\pic\\button_back.png");
    m_BackTexture.setSmooth(true);
    m_BackSprite.setTexture(m_BackTexture);
    m_BackSprite.setScale(screenHeight / m_BackTexture.getSize().y * 0.12f, 
        screenHeight / m_BackTexture.getSize().y * 0.12f);
    m_BackSprite.setPosition(0, 0);

    // Загружаем карточку 1 уровня
    m_OneLevelCardTexture.loadFromFile("..\\pic\\LevelOneCard.png");
    m_OneLevelCardTexture.setSmooth(true);
    m_OneLevelCardSprite.setTexture(m_OneLevelCardTexture);
    m_OneLevelCardSprite.setScale(screenHeight / m_OneLevelCardSprite.getGlobalBounds().height * 0.65f,
        screenHeight / m_OneLevelCardSprite.getGlobalBounds().height * 0.65f);
    m_OneLevelCardSprite.setPosition(5, (screenHeight - m_OneLevelCardSprite.getGlobalBounds().height) / 2.f);

    // Загружаем карточку 2 уровня
    m_TwoLevelCardTexture.loadFromFile("..\\pic\\LevelTwoCard.png");
    m_TwoLevelCardTexture.setSmooth(true);
    m_TwoLevelCardSprite.setTexture(m_TwoLevelCardTexture);
    m_TwoLevelCardSprite.setScale(screenHeight / m_TwoLevelCardSprite.getGlobalBounds().height * 0.65f,
        screenHeight / m_TwoLevelCardSprite.getGlobalBounds().height * 0.65f);
    m_TwoLevelCardSprite.setPosition(screenWidth / 2.0f - m_TwoLevelCardSprite.getGlobalBounds().width * 0.5f, 
        (screenHeight - m_TwoLevelCardSprite.getGlobalBounds().height) / 2.f);

    // Загружаем карточку 3 уровня
    m_ThreeLevelCardTexture.loadFromFile("..\\pic\\LevelThreeCard.png");
    m_ThreeLevelCardTexture.setSmooth(true);
    m_ThreeLevelCardSprite.setTexture(m_ThreeLevelCardTexture);
    m_ThreeLevelCardSprite.setScale(screenHeight / m_ThreeLevelCardSprite.getGlobalBounds().height * 0.65f,
        screenHeight / m_ThreeLevelCardSprite.getGlobalBounds().height * 0.65f);
    m_ThreeLevelCardSprite.setPosition(screenWidth - m_ThreeLevelCardSprite.getGlobalBounds().width - 5, 
        (screenHeight - m_ThreeLevelCardSprite.getGlobalBounds().height) / 2.f);

    starSize = Vector2f(500, 150);
    m_StarsTexture.loadFromFile("..\\pic\\starsCounter.png");
    m_StarsTexture.setSmooth(true);
    m_StarsSprite.setTexture(m_StarsTexture);
    m_StarsSprite.setScale(0.9f, 0.9f);

    m_LockTexture.loadFromFile("..\\pic\\lock.png");
    m_LockSprite.setTexture(m_LockTexture);
    m_LockSprite.setScale(0.6f, 0.6f);
}

void LevelMap::input(Event& event, float deltaTime, RenderWindow& m_Window)
{
    Vector2i mousePos = sf::Mouse::getPosition(m_Window);

    switch (event.type)
    {
    case Event::MouseButtonReleased:

        if (m_BackSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            newSceneId = oldSceneId;
            m_BackSprite.setColor(Color(255, 255, 255, 255));
        }
        if (m_OneLevelCardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_OneLevelCardSprite.setColor(Color(255, 255, 255, 255));
            newSceneId = mapping["level_one"];   // card one level
        }
        if (m_TwoLevelCardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) && iCanOpenSecondLevel)
        {
            m_TwoLevelCardSprite.setColor(Color(255, 255, 255, 255));
            newSceneId = mapping["level_two"];   // card two level
        }
        if (m_ThreeLevelCardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) && iCanOpenThirdLevel)
        {
            m_ThreeLevelCardSprite.setColor(Color(255, 255, 255, 255));
            newSceneId = mapping["level_three"];   // card three level
        }
        break;

    case Event::MouseMoved:
        if (m_OneLevelCardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_OneLevelCardSprite.setColor(Color(255, 255, 255, 200));
        }
        else
            m_OneLevelCardSprite.setColor(Color(255, 255, 255, 255));

        if (m_TwoLevelCardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_TwoLevelCardSprite.setColor(Color(255, 255, 255, 200));
        }
        else
            m_TwoLevelCardSprite.setColor(Color(255, 255, 255, 255));

        if (m_ThreeLevelCardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
        {
            m_ThreeLevelCardSprite.setColor(Color(255, 255, 255, 200));
        }
        else
            m_ThreeLevelCardSprite.setColor(Color(255, 255, 255, 255));

        if (m_BackSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            m_BackSprite.setColor(Color(255, 255, 255, 200));
        }
        else m_BackSprite.setColor(Color(255, 255, 255, 255));
        break;

    default:
        break;
    }

    if (!iCanOpenSecondLevel)
    {
        m_TwoLevelCardSprite.setColor(Color(255, 255, 255, 150));
    }

    if (!iCanOpenThirdLevel)
    {
        m_ThreeLevelCardSprite.setColor(Color(255, 255, 255, 150));
    }
}

void LevelMap::draw(float deltaTime, RenderWindow& m_Window)
{
    // Отрисовываем фон и кнопки
    m_Window.draw(m_BackgroundSprite);
    m_Window.draw(m_OneLevelCardSprite);
    m_Window.draw(m_TwoLevelCardSprite);
    m_Window.draw(m_ThreeLevelCardSprite);
    m_Window.draw(m_BackSprite);

    for (auto it = levelStars.begin(); it != levelStars.end(); ++it) 
    {
        int level = it->first;  // Ключ из map (уровень)
        int stars = it->second; // Значение из map (звезды)
        m_StarsSprite.setTextureRect(IntRect(0, stars * starSize.y, starSize.x, starSize.y));
        switch (level)
        {
        case 1:
            m_StarsSprite.setPosition(15, (screenHeight - m_OneLevelCardSprite.getGlobalBounds().height) / 2.f + 5);
            break;
        case 2:
            m_StarsSprite.setPosition((screenWidth - m_TwoLevelCardSprite.getGlobalBounds().width) / 2.f + 10,
                (screenHeight - m_TwoLevelCardSprite.getGlobalBounds().height) / 2.f + 5);
            break;
        case 3:
            m_StarsSprite.setPosition(screenWidth - m_ThreeLevelCardSprite.getGlobalBounds().width + 10,
                (screenHeight - m_ThreeLevelCardSprite.getGlobalBounds().height) / 2.f + 5);
            break;
        default:
            break;
        }
        if ((level == 2 && iCanOpenSecondLevel) || (level == 3 && iCanOpenThirdLevel) || level == 1)
            m_Window.draw(m_StarsSprite);
    }

    if (!iCanOpenSecondLevel)
    {
        m_LockSprite.setPosition((screenWidth - m_LockSprite.getGlobalBounds().width) / 2.0f,
            (screenHeight - m_LockSprite.getGlobalBounds().height) / 2.f);
        m_Window.draw(m_LockSprite);
    }
    if (!iCanOpenThirdLevel)
    {
        m_LockSprite.setPosition(screenWidth - m_ThreeLevelCardSprite.getGlobalBounds().width * 0.8f,
            (screenHeight - m_LockSprite.getGlobalBounds().height) / 2.f);
        m_Window.draw(m_LockSprite);
    }

}


int LevelMap::getCurrentScene()
{
    return mapping[scene];
}

int LevelMap::getNewScene()
{
    return newSceneId;
}