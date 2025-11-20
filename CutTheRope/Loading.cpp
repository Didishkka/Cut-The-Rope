#include "Loading.h"

Loading::Loading()
{
    scene = "loading";
    newSceneId = mapping["loading"];

    // Получаем разрешение экрана
    auto desktopMode = VideoMode::getDesktopMode();
    float screenWidth = static_cast<float>(desktopMode.width);
    float screenHeight = static_cast<float>(desktopMode.height);

    // Загружаем фон в текстуру
    if (!m_BackgroundTexture.loadFromFile("..\\pic\\background_loading.png"))
        throw std::runtime_error("Failed to load background texture");

    m_BackgroundSprite.setTexture(m_BackgroundTexture);
    m_BackgroundSprite.setScale(screenWidth / m_BackgroundTexture.getSize().x, screenHeight / m_BackgroundTexture.getSize().y);

    // Связываем текстуру загрузки и спрайт
    if (!m_LoadingTexture.loadFromFile("..\\pic\\loading.png"))
        throw std::runtime_error("Failed to load background texture");

    m_LoadingSprite.setTexture(m_LoadingTexture);
    m_LoadingSprite.setScale(screenWidth / m_LoadingTexture.getSize().x * 0.8f, screenWidth / m_LoadingTexture.getSize().x * 0.8f);
    m_LoadingTexture.setSmooth(true);
    Vector2i spriteSize(2000 * screenWidth / m_LoadingTexture.getSize().x * 0.8f, 500 * screenHeight / m_LoadingTexture.getSize().y * 0.8f);
    m_LoadingSprite.setTextureRect(IntRect(0, 0, spriteSize.x, spriteSize.y));
    m_LoadingSprite.setPosition((screenWidth - spriteSize.x) / 2,  (screenHeight - spriteSize.y) / 2);
}


void Loading::animation(float deltaTime)
{
    int frameNum = 6;
    float animationDuration = 2.5f;
    Vector2i spriteSize(2000, 500);
    
    elapsedTime += deltaTime;

    int animFrame = static_cast<int>((elapsedTime / animationDuration) * static_cast<float>(frameNum)) % frameNum;

    if (animFrame == 0 && isCycle)
    {
        newSceneId = mapping["level_map"];
    }
    else m_LoadingSprite.setTextureRect(IntRect(0, animFrame * spriteSize.y, spriteSize.x, spriteSize.y));

    if (animFrame > 0) isCycle = true;
}

void Loading::draw(float deltaTime, RenderWindow& m_Window)
{
    m_Window.draw(m_BackgroundSprite);
    animation(deltaTime);
    m_Window.draw(m_LoadingSprite);
}

int Loading::getCurrentScene()
{
    return mapping[scene];
}

int Loading::getNewScene()
{
    return newSceneId;
}