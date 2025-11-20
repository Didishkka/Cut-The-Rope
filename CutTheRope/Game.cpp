#include "Game.h"
#include <memory>
#include <iostream>

Game::Game()
{
    // Получаем разрешение экрана, создаем окно SFML и View
    Vector2i resolution;
    resolution.x = VideoMode::getDesktopMode().width;
    resolution.y = VideoMode::getDesktopMode().height;

    m_Window.create(VideoMode(resolution.x, resolution.y),
        "Cut the Rope",
        Style::Fullscreen);

    for (size_t i = 1; i <= 3; i++)
    {
        levelStars[i] = 0;
    }

    // Инициализация начальной сцены
    currentScene = std::make_unique<Start>();

    // Загружаем новый трек
    currentMusicFile = "..\\music\\menu_music.wav";
    if (!backgroundMusic.openFromFile(currentMusicFile))
    {
        std::cerr << "Ошибка загрузки музыки: " << currentMusicFile << std::endl;
        return;
    }

    // Настраиваем параметры музыки
    backgroundMusic.setLoop(true); // Зацикливание
    backgroundMusic.setVolume(50.f); // Громкость

    // Запускаем воспроизведение
    backgroundMusic.play();
}

void Game::start()
{
    // Расчет времени
    Clock clock;
    Event event;

    while (m_Window.isOpen())
    {
        // Перезапускаем таймер и записываем отмеренное время в dt
        Time dt = clock.restart();
        float dtAsSeconds = dt.asSeconds();

        if (dtAsSeconds > 0.1f) dtAsSeconds = 0.1f;  // Ограничение на время кадра

        changeCurrentScene(currentScene->getNewScene());

        // Обработка событий
        while (m_Window.pollEvent(event))
        {
            input(event, dtAsSeconds, m_Window);
            currentScene->input(event, dtAsSeconds, m_Window);
        }

        // Стираем предыдущий кадр
        m_Window.clear(Color::White);

        // Отрисовка текущей сцены
        currentScene->draw(dtAsSeconds, m_Window);

        // Отображаем все, что нарисовали
        m_Window.display();
    }
}

void Game::changeCurrentScene(int newScene)
{
    if (newScene == currentScene->getCurrentScene())
        return;

    // Сохраняем данные уровня, если переходим с него
    if (dynamic_cast<Level*>(currentScene.get())) 
    {
        auto level = static_cast<Level*>(currentScene.get());
        levelStars[level->getLevelNumber()] =
            std::max(levelStars[level->getLevelNumber()], level->getStarsCollected());
    }
    if (dynamic_cast<Settings*>(currentScene.get()))
    {
        auto settings = static_cast<Settings*>(currentScene.get());
        toggleMusic(settings->isMusicToggled());
        toggleSounds(settings->areSoundsToggled());
    }

    // Смена музыки для сцены
    playMusicForScene(newScene);

    // меняем сцену
    switch (newScene)
    {
    case 0:   // Начальное окно
        currentScene = std::make_unique<Start>();
        break;
    case 1:   // Фейк загрузка игры
        currentScene = std::make_unique<Loading>();
        break;
    case 2:   // Карта уровней (1, 2, 3 уровни)
        currentScene = std::make_unique<LevelMap>(levelStars);
        break;
    case 3:   // 1 уровень
        currentScene = std::make_unique<Level>(1, areSoundsEnabled);
        break;
    case 4:   // 2 уровень
        currentScene = std::make_unique<Level>(2, areSoundsEnabled);
        break;
    case 5:   // 3 уровень
        currentScene = std::make_unique<Level>(3, areSoundsEnabled);
        break;
    case 7:   // Настройки
        currentScene = std::make_unique<Settings>(isMusicEnabled, areSoundsEnabled);
        break;
    default:
        break;
    }
}

void Game::playMusicForScene(int scene)
{
    if (!isMusicEnabled)
    {
        backgroundMusic.stop();
        return;
    }
    std::string musicFile;

    // Выбираем файл музыки в зависимости от сцены
    switch (scene)
    {
    case 0: // Начальное окно
    case 7: // Настройки
        musicFile = "..\\music\\menu_music.wav";
        break;
    case 1: // Загрузка
        break;
    case 2: // Карта уровней
    case 3: // Уровень 1
    case 4: // Уровень 2
    case 5: // Уровень 3
        musicFile = "..\\music\\game_music.wav";
        break;
    default:
        return; // Если музыка для этой сцены не требуется
    }

    // Если музыка уже играет, ничего не делаем
    if (backgroundMusic.getStatus() == Music::Playing && 
        backgroundMusic.getPlayingOffset() > seconds(0) && 
        musicFile == currentMusicFile)
        return;

    // Останавливаем текущую музыку
    backgroundMusic.stop();

    if (musicFile == "")
        return;

    // Загружаем новый трек
    backgroundMusic.openFromFile(musicFile);

    // Настраиваем параметры музыки
    backgroundMusic.setLoop(true); // Зацикливание

    // Сохраняем текущий файл музыки
    currentMusicFile = musicFile;

    // Запускаем воспроизведение
    backgroundMusic.play();
}

void Game::input(Event& event, float deltaTime, RenderWindow& m_Window)
{
    if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
    {
        m_Window.close();
    }

    if (Keyboard::isKeyPressed(Keyboard::Up)) 
    {
        float volume = backgroundMusic.getVolume();
        backgroundMusic.setVolume(std::min(volume + 5.f, 100.f));
    }

    if (Keyboard::isKeyPressed(Keyboard::Down)) 
    {
        float volume = backgroundMusic.getVolume();
        backgroundMusic.setVolume(std::max(volume - 5.f, 0.f));
    }
}

void Game::toggleMusic(bool enable)
{
    isMusicEnabled = enable;

}

void Game::toggleSounds(bool enable)
{
    areSoundsEnabled = enable;
    // Для реализации: передавать это состояние в объекты, воспроизводящие звуки
}
