#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Platform.h"
#include "Coin.h"
#include <vector>
#include <string>

class Game {
public:
    Game();
    ~Game() = default;

    bool init();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();

    void handleCollisions();
    void centerCameraOnPlayer();
    void loadLevel(int index);
    void resetLevel();

    // IMPORTANT: windowW/windowH must be declared BEFORE window/camera
    const unsigned int windowW = 1280;
    const unsigned int windowH = 720;

    sf::RenderWindow window;
    sf::View camera;

    Player player;
    std::vector<Platform> platforms;
    std::vector<Coin> coins;

    sf::Font font;
    sf::Text hudText;
    sf::Text hintText;

    sf::RectangleShape backgroundLayer1;
    sf::RectangleShape backgroundLayer2;

    int score = 0;
    int collected = 0;
    bool showWin = false;
    bool paused = false;

    int currentLevel = 0;
    int maxLevels = 3;

    // --- Level-complete overlay ---
    bool levelComplete = false;           // true while showing "Level X Complete!" overlay
    float levelCompleteTimer = 0.f;       // seconds left on overlay
    float levelCompleteDuration = 1.5f;   // configurable duration
    sf::Text levelCompleteText;           // prebuilt text to render
};
