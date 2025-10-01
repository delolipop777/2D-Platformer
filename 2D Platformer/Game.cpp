#include "Game.h"
#include <sstream>

Game::Game()
    : window(sf::VideoMode(windowW, windowH), "2D Platformer"),
    camera(sf::FloatRect(0.f, 0.f, (float)windowW, (float)windowH)),
    player()
{
    window.setFramerateLimit(60);
}

bool Game::init()
{
    if (!font.loadFromFile("assets/arial.ttf")) {
        return false;
    }

    hudText.setFont(font);
    hudText.setCharacterSize(20);
    hudText.setFillColor(sf::Color::White);
    hudText.setPosition(10.f, 10.f);

    hintText.setFont(font);
    hintText.setCharacterSize(18);
    hintText.setFillColor(sf::Color::White);
    hintText.setPosition(10.f, 40.f);

    // prepare level-complete text (style will be updated on trigger)
    levelCompleteText.setFont(font);
    levelCompleteText.setCharacterSize(44);
    levelCompleteText.setFillColor(sf::Color::Yellow);
    levelCompleteText.setStyle(sf::Text::Bold);

    backgroundLayer1.setSize(sf::Vector2f(10000.f, 2000.f));
    backgroundLayer1.setFillColor(sf::Color(70, 150, 255));
    backgroundLayer1.setPosition(-2000.f, -1000.f);

    backgroundLayer2.setSize(sf::Vector2f(10000.f, 2000.f));
    backgroundLayer2.setFillColor(sf::Color(100, 200, 100));
    backgroundLayer2.setPosition(-2000.f, 200.f);

    loadLevel(0);
    return true;
}

void Game::run()
{
    sf::Clock clock;
    while (window.isOpen()) {
        processEvents();
        float dt = clock.restart().asSeconds();

        if (!paused && !showWin) {
            // if in level-complete overlay, only update the timer
            if (levelComplete) {
                levelCompleteTimer -= dt;
                if (levelCompleteTimer <= 0.f) {
                    // move to next level (or final win)
                    levelComplete = false;
                    currentLevel++;
                    if (currentLevel >= maxLevels) {
                        showWin = true;
                        hintText.setString("Congratulations! You beat all levels! Press R to restart.");
                    }
                    else {
                        loadLevel(currentLevel);
                    }
                }
            }
            else {
                update(dt);
            }
        }
        render();
    }
}

void Game::processEvents()
{
    sf::Event ev;
    while (window.pollEvent(ev)) {
        if (ev.type == sf::Event::Closed) window.close();
        if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::R) resetLevel();
            if (ev.key.code == sf::Keyboard::P) paused = !paused;
            if (ev.key.code == sf::Keyboard::Escape) window.close();

            // Allow skipping the overlay with Space or Enter (optional)
            if (levelComplete && (ev.key.code == sf::Keyboard::Space || ev.key.code == sf::Keyboard::Enter)) {
                levelCompleteTimer = 0.f; // will advance next loop
            }
        }
    }
}

void Game::update(float dt)
{
    // Normal gameplay update (player + collisions + camera + coins)
    player.update(dt);

    handleCollisions();
    centerCameraOnPlayer();

    // update animated coins
    for (auto& c : coins) c.update(dt);

    // If player collected all coins, trigger level-complete overlay (unless already triggered)
    if (!levelComplete && collected == static_cast<int>(coins.size()) && !showWin) {
        levelComplete = true;
        levelCompleteTimer = levelCompleteDuration;

        std::ostringstream ss;
        ss << "Level " << (currentLevel + 1) << " Complete!";
        levelCompleteText.setString(ss.str());

        // center the levelCompleteText on screen (we'll render in screen space later)
        sf::FloatRect bounds = levelCompleteText.getLocalBounds();
        levelCompleteText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        levelCompleteText.setPosition((float)windowW / 2.f, (float)windowH / 2.f);

        // small hint
        hintText.setString("Advancing to next level...");
    }
}

void Game::render()
{
    window.clear();

    // world view
    window.setView(camera);

    sf::Vector2f camPos = camera.getCenter();
    backgroundLayer1.setPosition(camPos.x * 0.2f - 5000.f, camPos.y * 0.1f - 1000.f);
    backgroundLayer2.setPosition(camPos.x * 0.5f - 5000.f, camPos.y * 0.2f - 200.f);

    window.draw(backgroundLayer1);
    window.draw(backgroundLayer2);

    for (auto& p : platforms) p.draw(window);
    for (auto& c : coins) c.draw(window);
    player.draw(window);

    // HUD and overlays in screen space
    window.setView(window.getDefaultView());

    std::ostringstream ss;
    ss << "Score: " << score << "   Coins: " << collected << " / " << coins.size();
    if (paused) ss << "   [PAUSED]";
    hudText.setString(ss.str());
    window.draw(hudText);

    window.draw(hintText);

    // Draw level-complete overlay if active
    if (levelComplete) {
        // semi-transparent dark overlay
        sf::RectangleShape overlay(sf::Vector2f((float)windowW, (float)windowH));
        overlay.setFillColor(sf::Color(0, 0, 0, 140));
        overlay.setPosition(0.f, 0.f);
        window.draw(overlay);

        window.draw(levelCompleteText);
    }

    // Final win message
    if (showWin) {
        sf::Text winText;
        winText.setFont(font);
        winText.setCharacterSize(48);
        winText.setFillColor(sf::Color::Yellow);
        winText.setStyle(sf::Text::Bold);
        winText.setString("YOU WIN!");
        sf::FloatRect bounds = winText.getLocalBounds();
        winText.setPosition((float)windowW / 2.f - bounds.width / 2.f, (float)windowH / 2.f - bounds.height / 2.f);
        window.draw(winText);
    }

    window.display();
}

void Game::handleCollisions()
{
    // Platform collisions (vertical then horizontal)
    bool onGround = false;
    sf::FloatRect pbox = player.getBounds();

    for (auto& plat : platforms) {
        sf::FloatRect pb = plat.getBounds();
        if (pbox.intersects(pb)) {
            sf::FloatRect inter;
            pbox.intersects(pb, inter);

            float playerBottom = player.position.y + player.getSize().y / 2.f;
            float playerTop = player.position.y - player.getSize().y / 2.f;
            float platTop = plat.position.y - plat.getSize().y / 2.f;
            float platBottom = plat.position.y + plat.getSize().y / 2.f;

            if (player.velocity.y >= 0 && (playerBottom - inter.height) <= platTop + 1.f) {
                player.position.y = platTop - player.getSize().y / 2.f;
                player.velocity.y = 0.f;
                player.jumpCount = 0;
                onGround = true;
                pbox = player.getBounds();
            }
            else if (player.velocity.y < 0 && (playerTop + inter.height) >= platBottom - 1.f) {
                player.position.y = platBottom + player.getSize().y / 2.f + 0.1f;
                player.velocity.y = 0.f;
                pbox = player.getBounds();
            }
        }
    }

    // Horizontal push-out
    for (auto& plat : platforms) {
        sf::FloatRect pb = plat.getBounds();
        if (player.getBounds().intersects(pb)) {
            sf::FloatRect inter;
            player.getBounds().intersects(pb, inter);
            if (inter.width < inter.height) {
                if (player.position.x < plat.position.x) player.position.x -= inter.width;
                else player.position.x += inter.width;
                player.velocity.x = 0.f;
            }
        }
    }

    player.onGround = onGround;
    player.setPosition(player.position);

    // Coin pickup
    for (auto& c : coins) {
        if (!c.isCollected() && c.getBounds().intersects(player.getBounds())) {
            c.collect();
            score += c.value;
            collected++;
            player.velocity.y = -200.f;
        }
    }
}

void Game::centerCameraOnPlayer()
{
    sf::Vector2f target(player.position.x, (float)windowH / 2.f);
    if (target.x < (float)windowW / 2.f) target.x = (float)windowW / 2.f;
    camera.setCenter(target);
}

void Game::loadLevel(int index)
{
    platforms.clear();
    coins.clear();
    score = 0;
    collected = 0;
    showWin = false;
    paused = false;
    levelComplete = false;
    levelCompleteTimer = 0.f;

    switch (index) {
    case 0:
        platforms.emplace_back(0.f, 680.f, 4000.f, 80.f);
        platforms.emplace_back(200.f, 560.f, 200.f, 24.f);
        platforms.emplace_back(500.f, 450.f, 200.f, 24.f);
        platforms.emplace_back(850.f, 380.f, 160.f, 24.f);
        coins.emplace_back(250.f, 500.f);
        coins.emplace_back(550.f, 390.f);
        coins.emplace_back(900.f, 320.f);
        player.resetTo({ 100.f, 600.f });
        hintText.setString("Level 1: Collect all coins!");
        break;

    case 1:
        platforms.emplace_back(0.f, 680.f, 4000.f, 80.f);
        platforms.emplace_back(300.f, 560.f, 150.f, 24.f);
        platforms.emplace_back(700.f, 460.f, 180.f, 24.f);
        platforms.emplace_back(1100.f, 360.f, 150.f, 24.f);
        platforms.emplace_back(1500.f, 260.f, 200.f, 24.f);
        coins.emplace_back(320.f, 520.f);
        coins.emplace_back(720.f, 400.f);
        coins.emplace_back(1120.f, 300.f);
        player.resetTo({ 100.f, 600.f });
        hintText.setString("Level 2: Watch your jumps!");
        break;

    case 2:
        platforms.emplace_back(0.f, 680.f, 5000.f, 80.f);
        platforms.emplace_back(400.f, 560.f, 150.f, 24.f);
        platforms.emplace_back(850.f, 480.f, 140.f, 24.f);
        platforms.emplace_back(1250.f, 380.f, 160.f, 24.f);
        platforms.emplace_back(1650.f, 280.f, 180.f, 24.f);
        coins.emplace_back(420.f, 500.f);
        coins.emplace_back(870.f, 420.f);
        coins.emplace_back(1290.f, 340.f);
        coins.emplace_back(1690.f, 260.f);
        player.resetTo({ 100.f, 600.f });
        hintText.setString("Level 3: Final challenge!");
        break;

    default:
        currentLevel = 0;
        loadLevel(0);
        break;
    }
}

void Game::resetLevel()
{
    currentLevel = 0;
    loadLevel(currentLevel);
}
