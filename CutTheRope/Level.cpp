#include "Level.h"
#include <iostream>
#include <windows.h>

// Получаем разрешение экрана

Level::Level(int id, bool sounds) : levelId(id), areSoundsEnabled(sounds)
{
    auto desktopMode = VideoMode::getDesktopMode();
    screenWidth = static_cast<float>(desktopMode.width);
    screenHeight = static_cast<float>(desktopMode.height);

    scene = "level";
    newSceneId = mapping[scene];
    oldSceneId = mapping["level_map"];

    font.loadFromFile("..\\fonts\\Polysoup.otf");

    // Выполняем махинации для синхронизации деталей
    rope.setLevelId(levelId);
    candy.setAmNyamSize(Vector2f(character.getSize().x, character.getSize().y));
    candy.to(rope.getLastNode().position.x, rope.getLastNode().position.y);
    hairDryer.to(screenWidth * 3.f / 4.f, screenHeight / 1.9f);
    character.toggleSounds(areSoundsEnabled);
    rope.toggleSounds(areSoundsEnabled);
    bubble.toggleSounds(areSoundsEnabled);
    hairDryer.toggleSounds(areSoundsEnabled);
     
    // Подргрузка текстур и спрайтов
    ////////////////////////////////
    m_BackgroundTexture.loadFromFile("..\\pic\\background_game.jpg");
    m_BackgroundTexture.setSmooth(true);
    m_BackgroundSprite.setTexture(m_BackgroundTexture);
    m_BackgroundSprite.setScale(screenWidth / m_BackgroundTexture.getSize().x, screenHeight / m_BackgroundTexture.getSize().y);

    m_FinalTexture.loadFromFile("..\\pic\\final.png");
    m_FinalSprite.setTexture(m_FinalTexture);
    m_FinalSprite.setScale(screenWidth / m_FinalTexture.getSize().x, screenHeight / m_FinalTexture.getSize().y);

    m_SeatTexture.loadFromFile("..\\pic\\doska.png");
    m_SeatSprite.setTexture(m_SeatTexture);
    m_SeatSprite.setScale(screenWidth / m_BackgroundTexture.getSize().x * 0.5, screenHeight / m_BackgroundTexture.getSize().y * 0.5);
    m_SeatSprite.setPosition((screenWidth - m_SeatSprite.getGlobalBounds().width) / 2.f, 
        screenHeight - m_SeatSprite.getGlobalBounds().height - character.getSprite().getGlobalBounds().height / 8.f);

    m_BackTexture.setSmooth(true);
    m_BackTexture.loadFromFile("..\\pic\\button_back.png");
    m_BackSprite.setTexture(m_BackTexture);
    m_BackSprite.setScale(screenHeight / m_BackTexture.getSize().y * 0.12f, 
        screenHeight / m_BackTexture.getSize().y * 0.12f);
    m_BackSprite.setPosition(0, 0);

    m_RetryTexture.loadFromFile("..\\pic\\button_retry.png");
    m_RetryTexture.setSmooth(true);
    m_RetrySprite.setTexture(m_RetryTexture);
    m_RetrySprite.setScale(screenHeight / m_RetryTexture.getSize().y * 0.12f, screenHeight / m_RetryTexture.getSize().y * 0.12f);
    m_RetrySprite.setPosition(0, m_BackSprite.getGlobalBounds().height);

    m_BarrierTexture.loadFromFile("..\\pic\\barrier.png");
    m_BarrierTexture.setSmooth(true);
    m_BarrierSprite.setTexture(m_BarrierTexture);
    m_BarrierSprite.setScale(0.8f, 0.8f);

    m_TrampolineTexture.loadFromFile("..\\pic\\trampoline.png");
    m_TrampolineTexture.setSmooth(true);
    m_TrampolineSprite.setTexture(m_TrampolineTexture);
    m_TrampolineSprite.setScale(0.55f, 0.55f);
    m_TrampolineSprite.setPosition(0.f, 0.f);


    m_StarTexture.loadFromFile("..\\pic\\star.png");
    Vector2f starSize(420, 400);
    m_1StarSprite.setTexture(m_StarTexture);
    m_1StarSprite.setTextureRect(IntRect(0, 0, starSize.x, starSize.y));
    m_1StarSprite.setScale(0.2f, 0.2f);
    Vector2f candyPos(candy.getPosition().x, candy.getSprite().getGlobalBounds().height + candy.getPosition().y);
    m_1StarSprite.setOrigin(m_1StarSprite.getGlobalBounds().width / 2.f, 0);
    m_2StarSprite.setTexture(m_StarTexture);
    m_2StarSprite.setTextureRect(IntRect(0, 0, starSize.x, starSize.y));
    m_2StarSprite.setScale(0.2f, 0.2f);
    m_2StarSprite.setOrigin(m_1StarSprite.getGlobalBounds().width / 2.f, 0);
    m_3StarSprite.setTexture(m_StarTexture);
    m_3StarSprite.setTextureRect(IntRect(0, 0, starSize.x, starSize.y));
    m_3StarSprite.setScale(0.2f, 0.2f);
    m_3StarSprite.setOrigin(m_1StarSprite.getGlobalBounds().width / 2.f, 0);

    // Звуковые файлы
    if (!musicStar1.openFromFile("..\\music\\star_1.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;
    if (!musicStar2.openFromFile("..\\music\\star_2.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;
    if (!musicStar3.openFromFile("..\\music\\star_3.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;
    if (!musicWords.openFromFile("..\\music\\star_3.wav"))
        std::cerr << "Ошибка загрузки monster_sad.wav" << std::endl;

    // Настройка громкости
    musicStar1.setVolume(50);
    musicStar2.setVolume(50);
    musicStar3.setVolume(50);
    musicWords.setVolume(50);

    switch (levelId)
    {
    case 1:
        bubble.to(0, 0);
        m_BarrierSprite.setPosition(0, 0);

        m_1StarSprite.setPosition(candyPos.x - m_1StarSprite.getGlobalBounds().width / 4.f,
            candyPos.y + m_1StarSprite.getGlobalBounds().height * 0.5f);
        m_2StarSprite.setPosition(candyPos.x - m_1StarSprite.getGlobalBounds().width / 4.f,
            candyPos.y + m_1StarSprite.getGlobalBounds().height * 2.f);
        m_3StarSprite.setPosition(candyPos.x - m_1StarSprite.getGlobalBounds().width / 4.f,
            candyPos.y + m_1StarSprite.getGlobalBounds().height * 3.5f);
        break;
    case 2:
        m_BarrierSprite.setRotation(25.f);
        m_BarrierSprite.setPosition(m_BarrierSprite.getGlobalBounds().width / 4.f,
            (screenHeight - m_BarrierSprite.getGlobalBounds().height) / 2.f);
        m_TrampolineSprite.setPosition(screenWidth / 3.8f, screenHeight - m_TrampolineSprite.getGlobalBounds().height);

        m_1StarSprite.setPosition(candyPos.x - m_1StarSprite.getGlobalBounds().width / 4.f,
            candyPos.y + m_1StarSprite.getGlobalBounds().height * 0.5f);
        m_2StarSprite.setPosition(m_TrampolineSprite.getPosition().x + m_TrampolineSprite.getGlobalBounds().width / 2.f,
            m_TrampolineSprite.getPosition().y - 100);
        m_3StarSprite.setPosition(character.getSprite().getPosition().x + character.getSize().x / 3.f,
            character.getSprite().getPosition().y - 100);
        break;
    case 3:
        m_BarrierSprite.setRotation(335.f);
        m_BarrierSprite.setPosition(screenWidth - m_BarrierSprite.getGlobalBounds().width * 1.5f,
            (screenHeight - m_BarrierSprite.getGlobalBounds().height) * 2.f / 3.f);

        bubble.to(m_BarrierSprite.getPosition().x - bubble.getRadius() * 1.5f,
            m_BarrierSprite.getPosition().y + bubble.getRadius() / 1.5f);
        //m_TrampolineSprite.setPosition(screenWidth / 3.8f, screenHeight - m_TrampolineSprite.getGlobalBounds().height);

        m_1StarSprite.setPosition(character.getSprite().getPosition().x + character.getSize().x / 3.f,
            m_1StarSprite.getGlobalBounds().height);
        m_2StarSprite.setPosition(m_BarrierSprite.getPosition().x, m_BarrierSprite.getPosition().y - 100);
        m_3StarSprite.setPosition(character.getSprite().getPosition().x + character.getSize().x / 3.f,
            character.getSprite().getPosition().y - 100);
        break;
    default:
        break;
    }

    m_WordTexture.loadFromFile("..\\pic\\tapHere.png");
    m_WordSprite.setTexture(m_WordTexture);
    m_WordSprite.setScale(0.5f, 0.5f);
    m_WordSprite.setPosition(screenWidth - m_WordSprite.getGlobalBounds().width * 1.2f,
        screenHeight - m_WordSprite.getGlobalBounds().height * 1.2f);

    ////////////////////////////////////////
    candy.setBarrier(m_BarrierSprite);
    candy.setTrampoline(m_TrampolineSprite);
}

void Level::input(Event& event, float deltaTime, RenderWindow& m_Window)
{
    Vector2i mousePos = sf::Mouse::getPosition(m_Window);

    rope.input(event, deltaTime, m_Window);
    hairDryer.input(event, m_Window);
    if (event.type == Event::MouseButtonReleased)
    {
        if (m_BackSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            newSceneId = oldSceneId;
            if (!candy.fallen())
                starCounter = 0;
            m_BackSprite.setColor(Color(255, 255, 255, 255));
        }
        
        if (m_RetrySprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            newSceneId = levelId + 2;
            if (!candy.fallen())
                starCounter = 0;
            m_RetrySprite.setColor(Color(255, 255, 255, 255));
        }

        if (bubble.getSprite().getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            if (bubble.containsCandy(candy.getPosition()))
            {
                bubble.pop();
            }
        }
    }
    else if (event.type == Event::MouseMoved)
    {
        if (m_BackSprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            m_BackSprite.setColor(Color(255, 255, 255, 200));
        }
        else m_BackSprite.setColor(Color(255, 255, 255, 255));

        if (m_RetrySprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            m_RetrySprite.setColor(Color(255, 255, 255, 200));
        }
        else m_RetrySprite.setColor(Color(255, 255, 255, 255));
    }
}
 //рисует звезды в игре
void Level::drawStars(RenderWindow& m_Window)
{
    Vector2f candyPos(candy.getPosition().x, candy.getPosition().y);
    FloatRect c = candy.getSprite().getGlobalBounds();
    FloatRect s1 = m_1StarSprite.getGlobalBounds();
    FloatRect s2 = m_2StarSprite.getGlobalBounds();
    FloatRect s3 = m_3StarSprite.getGlobalBounds();


    if (c.intersects(s1) && !isFirstStarClear)
    {
        isFirstStarClear = true;
        starCounter++;
    }
    if (c.intersects(s2) && !isSecondStarClear)
    {
        isSecondStarClear = true;
        starCounter++;
    }
    if (c.intersects(s3) && !isThirdStarClear)
    {
        isThirdStarClear = true;
        starCounter++;
    }
    if (candyPos.x >= screenWidth || candyPos.y >= screenHeight ||
        candyPos.x <= 0 || candyPos.y <= 0)
    {
        if (!candy.eatten())
        {
            starCounter = 0;
        }
    }

    if (!isFirstStarClear)
        m_Window.draw(m_1StarSprite);
    if (!isSecondStarClear)
        m_Window.draw(m_2StarSprite);
    if (!isThirdStarClear)
        m_Window.draw(m_3StarSprite);
}

//рисует финальную сцену с подсчетом звезд
void Level::drawFinal(float deltaTime, RenderWindow& m_Window)
{
    stopTime += deltaTime;
    m_Window.draw(m_FinalSprite);

    
    if (starCounter > 0 && stopTime > 0.5f)
    {
        if (musicStar1.getStatus() != Music::Playing && areSoundsEnabled)
        {
            musicStar1.play();
        }
        m_Window.draw(m_1StarSprite);
    }
    if (starCounter > 1 && stopTime > 1.5f)
    {
        if (musicStar2.getStatus() != Music::Playing && areSoundsEnabled)
        {
            musicStar2.play();
        }
        m_Window.draw(m_2StarSprite);
    }
    if (starCounter > 2 && stopTime > 2.5f)
    {
        if (musicStar3.getStatus() != Music::Playing && areSoundsEnabled)
        {
            musicStar3.play();
        }
        m_Window.draw(m_3StarSprite);
    }
    if (stopTime > starCounter + 0.5f)
    {
        musicWords.setPitch(1.5);
        if (musicWords.getStatus() != Music::Playing && areSoundsEnabled)
        {
            musicWords.play();
        }
        m_Window.draw(m_WordSprite);
        displayLevelTime(m_Window, font, levelTimeAsSeconds);
    }
}

void Level::changeStarScale()
{
    if (!candy.fallen())
        return;

    m_WordTexture.loadFromFile("..\\pic\\words.png");
    m_WordSprite.setTexture(m_WordTexture);
    m_WordSprite.setScale(2.f, 2.f);
    m_WordSprite.setPosition((screenWidth - m_WordSprite.getGlobalBounds().width) / 2.f,
        m_WordSprite.getGlobalBounds().height / 8.f);

    Vector2f starSize(400, 400);
    m_1StarSprite.setScale(1.3f, 1.3f);
    m_2StarSprite.setScale(1.3f, 1.3f);
    m_3StarSprite.setScale(1.3f, 1.3f);
    m_1StarSprite.setTextureRect(IntRect(0, 2 * starSize.y, starSize.x, starSize.y));
    m_2StarSprite.setTextureRect(IntRect(0, 2 * starSize.y, starSize.x, starSize.y));
    m_3StarSprite.setTextureRect(IntRect(0, 2 * starSize.y, starSize.x, starSize.y));

    m_1StarSprite.setOrigin(0, 0);
    m_2StarSprite.setOrigin(0, 0);
    m_3StarSprite.setOrigin(0, 0);
    

    starSize = Vector2f(m_1StarSprite.getGlobalBounds().width, m_1StarSprite.getGlobalBounds().height);

    m_1StarSprite.setPosition(10, screenHeight - starSize.y + 16);
    m_2StarSprite.setPosition((screenWidth - starSize.x) / 2.f, screenHeight * 3 / 4.f - starSize.y + 16);
    m_3StarSprite.setPosition(screenWidth - starSize.x - 10, screenHeight - starSize.y + 16);

    Vector2f wordSize(500, 100);
    m_WordSprite.setTextureRect(IntRect(0, starCounter * wordSize.y, wordSize.x, wordSize.y));
}

void Level::setBoardFade()
{
    if (time > 3.f && !startFade)
    {
        time = 0.f;
        startFade = true;
    }

    if (startFade)
    {
        if (time > 0.5f)
        {
            count++;
            time = 0.f;
            if (count == 5)
            {
                m_BarrierSprite.setColor(Color(255, 255, 255, 0));
            }
            else if (count < 5)
                m_BarrierSprite.setColor(Color(255, 255, 255, (count % 2 == 0 ? 255 : 255 / 2)));
        }
        if (count == 10)
        {
            time = 0.f;
            count = 0;
            startFade = false;
            m_BarrierSprite.setColor(Color(255, 255, 255, 255));
        }
    }
}

void Level::draw(float deltaTime, RenderWindow& m_Window)
{
    if (!candy.fallen())
    {
        m_Window.draw(m_BackgroundSprite);
        m_Window.draw(m_SeatSprite);
        rope.draw(deltaTime, m_Window);
        m_Window.draw(m_BackSprite);
        m_Window.draw(m_RetrySprite);
        character.setCandyPos(candy.getPosition().x, candy.getPosition().y);
        character.draw(deltaTime, m_Window);
        
        drawStars(m_Window);
        if (levelId == 2)
        {
            time += deltaTime;
            setBoardFade();
            if (m_BarrierSprite.getColor().a == 0)
            {
                m_BarrierSprite.setPosition(0, 0);
                candy.setBarrier(m_BarrierSprite);
            }
            else
            {
                m_BarrierSprite.setPosition(m_BarrierSprite.getGlobalBounds().width / 4.f,
                    (screenHeight - m_BarrierSprite.getGlobalBounds().height) / 2.f);
                candy.setBarrier(m_BarrierSprite);
                m_Window.draw(m_BarrierSprite);
            }
            m_Window.draw(m_TrampolineSprite);
        }

        candy.draw(m_Window);
        candy.update(deltaTime, rope, bubble);

        if (levelId == 3)
        {
            bubble.setAirFlowActive(hairDryer.isAirFlowActive());
            if (hairDryer.isAirFlowActive()) {
                // Если поток активен, задаём направление (например, вверх)
                bubble.setAirFlowDirection({ 0.1f, -1.f }, 50.f); // Небольшой угол и сила потока
            }
            bubble.update(deltaTime);
            bubble.draw(m_Window);
            m_Window.draw(m_BarrierSprite);
            hairDryer.draw(m_Window);
            m_Window.draw(m_WordSprite);
            //m_Window.draw(m_TrampolineSprite);
        }
        changeStarScale();
        Time levelTime = clock.restart();
        levelTimeAsSeconds += levelTime.asSeconds();
    }
    else
    {
        drawFinal(deltaTime, m_Window);
        m_Window.draw(m_BackSprite);
        m_Window.draw(m_RetrySprite);
    }
}

int Level::getCurrentScene()
{
    return mapping[scene];
}

int Level::getNewScene()
{
    return newSceneId;
}

void Level::displayLevelTime(RenderWindow& m_Window, Font& font, float elapsedTime)
{
    // Разделяем время на минуты и секунды
    int minutes = static_cast<int>(elapsedTime) / 60;
    int seconds = static_cast<int>(elapsedTime) % 60;

    // Форматируем строку
    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << minutes << ":"
        << std::setfill('0') << std::setw(2) << seconds;

    // Создаем текст
    Text timeText;
    timeText.setFont(font);
    timeText.setString("Time: " + timeStream.str());
    timeText.setCharacterSize(100); // Размер текста
    timeText.setFillColor(Color::Black);
    timeText.setStyle(Text::Bold);
    timeText.setPosition(screenWidth / 10.f, screenHeight / 6.f); // Позиция на экране

    // Рисуем текст на окне
    m_Window.draw(timeText);
}