#include "Settings.h"

Settings::Settings(bool musicEnabled, bool soundsEnabled)
    : musicEnabled(musicEnabled), soundsEnabled(soundsEnabled)
{
    scene = "settings";
    newSceneId = mapping["settings"];
    oldSceneId = mapping["start"];

    // Получаем разрешение экрана
    auto desktopMode = VideoMode::getDesktopMode();
    screenWidth = static_cast<float>(desktopMode.width);
    screenHeight = static_cast<float>(desktopMode.height);

    // Настраиваем текстовые метки для кнопок
    font.loadFromFile("..\\fonts\\Polysoup.otf");
    musicToggleText.setFont(font); // Используйте заранее загруженный шрифт
    musicToggleText.setString("music");
    musicToggleText.setPosition(screenWidth / 4.f + 100, screenHeight / 7.25f);
    musicToggleText.setCharacterSize(300);

    soundsToggleText.setFont(font);
    soundsToggleText.setString("sounds");
    soundsToggleText.setPosition(screenWidth / 4.f + 100, screenHeight / 1.75f);
    soundsToggleText.setCharacterSize(300);
    
    boolSize = Vector2f(500, 500);

    m_BoolTexture.loadFromFile("..\\pic\\bool.png");
    m_Bool1Sprite.setTexture(m_BoolTexture);
    m_Bool1Sprite.setPosition(5, screenHeight / 7.f);
    m_Bool1Sprite.setScale(0.8f, 0.8f);
    m_Bool2Sprite.setTexture(m_BoolTexture);
    m_Bool2Sprite.setPosition(5, screenHeight / 1.8f);
    m_Bool2Sprite.setScale(0.8f, 0.8f);


    if (!m_BackgroundTexture.loadFromFile("..\\pic\\background_simple.png"))
        throw std::runtime_error("Failed to load background texture");

    m_BackgroundSprite.setTexture(m_BackgroundTexture);
    m_BackgroundSprite.setScale(screenWidth / m_BackgroundTexture.getSize().x, screenHeight / m_BackgroundTexture.getSize().y);

    if (!m_ButBack_Texture.loadFromFile("..\\pic\\button_back.png"))
        throw std::runtime_error("Failed to load background texture");

    m_ButBack_Texture.setSmooth(true);
    m_ButBack_Sprite.setTexture(m_ButBack_Texture);
    m_ButBack_Sprite.setScale(screenHeight / m_ButBack_Texture.getSize().y * 0.12f, 
        screenHeight / m_ButBack_Texture.getSize().y * 0.12f);
    m_ButBack_Sprite.setPosition(0, 0);
}

void Settings::input(Event& event, float deltaTime, RenderWindow& m_Window)
{
    Vector2i mousePos = sf::Mouse::getPosition(m_Window);

    if (event.type == Event::MouseButtonPressed)
    {
        if (m_ButBack_Sprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            newSceneId = oldSceneId;
            m_ButBack_Sprite.setColor(Color(255, 255, 255, 255));
        }
        if (m_Bool1Sprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            musicEnabled = !musicEnabled;
        }
        if (m_Bool2Sprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            soundsEnabled = !soundsEnabled;
        }
    }
    if (event.type == Event::MouseMoved)
    {
        if (m_ButBack_Sprite.getGlobalBounds().contains(static_cast<Vector2f>(mousePos)))
        {
            m_ButBack_Sprite.setColor(Color(255, 255, 255, 200));
        }
        else m_ButBack_Sprite.setColor(Color(255, 255, 255, 255));
    }
}

void Settings::draw(float deltaTime, RenderWindow& m_Window)
{
    m_Window.draw(m_BackgroundSprite);
    m_Window.draw(m_ButBack_Sprite);
    m_Window.draw(musicToggleText);
    m_Window.draw(soundsToggleText);
    m_Bool1Sprite.setTextureRect(IntRect(0, musicEnabled * boolSize.y, boolSize.x, boolSize.y));
    m_Window.draw(m_Bool1Sprite);
    m_Bool2Sprite.setTextureRect(IntRect(0, soundsEnabled * boolSize.y, boolSize.x, boolSize.y));
    m_Window.draw(m_Bool2Sprite);
}

int Settings::getCurrentScene()
{
    return mapping[scene];
}

int Settings::getNewScene()
{
    return newSceneId;
}