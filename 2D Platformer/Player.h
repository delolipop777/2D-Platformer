#pragma once
#include <SFML/Graphics.hpp>

class Player
{
private:
    // Core attributes
    int health;
    int score;

    // Physics
    sf::Vector2f position;
    sf::Vector2f velocity;

    // Graphics
    sf::RectangleShape m_shape;

    // Jump control
    int jumpCount;
    int maxJumps;
    bool jumpKeyPressedLastFrame = false;

    // Movement parameters (tunable)
    float maxSpeed;          // max horizontal speed
    float accel;             // horizontal acceleration
    float decel;             // horizontal deceleration
    float jumpStrength;      // upward impulse
    float gravity;           // pixels/s²
    float terminalVelocity;  // maximum fall speed

public:
    Player();

    // Gameplay
    int getHealth() const { return health; }
    int getScore() const { return score; }
    void takeDamage(int damage) { health = std::max(0, health - damage); }
    void addScore(int points) { score += points; }

    // Update & render
    void update(float dt);
    void draw(sf::RenderWindow& window) { window.draw(m_shape); }

    // Movement
    void handleInput(float dt);
    void jump();
};
