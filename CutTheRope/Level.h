#pragma once

#include "BaseScene.h"
#include "AmNyam.h"
#include "Rope.h"
#include "Candy.h"
#include "Bubble.h"
#include "HairDryer.h"
#include <string>
#include <iomanip>
#include <sstream>

using namespace sf;

class Level : public BaseScene
{
private:
    float screenWidth, screenHeight;

    AmNyam character;
    Rope rope;
    Candy candy;
    Bubble bubble;
    HairDryer hairDryer;

    // Позиция Конфеты
    Vector2f m_Position;

    // Объявляем спрайт и текстуру для фона
    Sprite m_BackgroundSprite;
    Texture m_BackgroundTexture;

    Sprite m_FinalSprite;
    Texture m_FinalTexture;

    Sprite m_SeatSprite;
    Texture m_SeatTexture;

    Texture m_StarTexture;
    Sprite m_1StarSprite;
    Sprite m_2StarSprite;
    Sprite m_3StarSprite;

    Texture m_WordTexture;
    Sprite m_WordSprite;

    Sprite m_BackSprite;
    Texture m_BackTexture;

    Sprite m_RetrySprite;
    Texture m_RetryTexture;

    Sprite m_BarrierSprite;
    Texture m_BarrierTexture;

    Texture m_TrampolineTexture;
    Sprite m_TrampolineSprite;

    float time = 3.f; // Время для доски на 2 уровне
    float stopTime = 0.f; // Время для задержки вывода количества звезд
    Clock clock; // Время прохождения уровня
    float levelTimeAsSeconds = 0.f;

    Font font;

    Music musicStar1;
    Music musicStar2;
    Music musicStar3;
    Music musicWords;

    bool startFade = false; // Флаг для начала исчезновени доски
    int count = 2; // Счетчик мерцания доски
    void setBoardFade();

    void drawStars(RenderWindow& m_Window);
    void drawFinal(float deltaTime, RenderWindow& m_Window);
    bool isFirstStarClear = false, isSecondStarClear = false, isThirdStarClear = false;
    int starCounter = 0;
    void changeStarScale();
    int levelId = 0;
    void displayLevelTime(RenderWindow& m_Window, Font& font, float elapsedTime);
    bool areSoundsEnabled = true;

public:

    Level() = default;
    Level(int id, bool sounds);
    void input(Event& event, float deltaTime, RenderWindow& m_Window) override;
    void draw(float deltaTime, RenderWindow& m_Window) override;
    int getCurrentScene() override;
    int getNewScene() override;
    int getStarsCollected() const { return starCounter; }
    int getLevelNumber() const { return levelId; }
};