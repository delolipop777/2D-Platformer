#pragma once
#include <SFML/Graphics.hpp>

class Player {
public:
    Player();

    void resetTo(const sf::Vector2f& pos);
    void update(float dt);
    void draw(sf::RenderWindow& win) const;
    void setPosition(const sf::Vector2f& pos);

    sf::FloatRect getBounds() const;
    sf::Vector2f getSize() const;

    // exposed for Game collision handling
    sf::Vector2f position;
    sf::Vector2f velocity;
    int jumpCount = 0;
    bool onGround = false;

private:
    void handleInput(float dt);
    sf::RectangleShape shape;

    // physics params
    float maxSpeed;
    float accel;
    float decel;
    float jumpStrength;
    float gravity;
    float terminalVelocity;
};
