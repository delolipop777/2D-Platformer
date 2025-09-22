#include "Player.h"

Player::Player()
    : health(100), score(0), position(400.0f, 500.0f), velocity(0.0f, 0.0f)
{
    m_shape.setSize(sf::Vector2f(50.0f, 50.0f));
    m_shape.setFillColor(sf::Color::Blue);
    m_shape.setPosition(position);

    // Tunable physics parameters
    maxSpeed = 200.f;       // max horizontal speed
    accel = 600.0f;          // horizontal acceleration
    decel = 800.0f;          // horizontal deceleration
    jumpStrength = -400.0f;  // upward impulse
    gravity = 900.0f;        // pixels/s²
    terminalVelocity = 600.0f;
    jumpCount = 0;
}

void Player::handleInput(float dt) {
    // Horizontal movement

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) velocity.x -= accel * dt;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) velocity.x += accel * dt;
    else {
        // Apply deceleration when no input
        if (velocity.x > 0) {
            velocity.x -= decel * dt;
            if (velocity.x < 0) velocity.x = 0;
        }
        else if (velocity.x < 0) {
            velocity.x += decel * dt;
            if (velocity.x > 0) velocity.x = 0;
        }
    }

    // Clamp horizontal speed
    if (velocity.x > maxSpeed) velocity.x = maxSpeed;
    if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;

    // Jump
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && jumpCount != 2) {
        velocity.y = jumpStrength;
        jumpCount++;
    }
}

void Player::update(float dt) {
    handleInput(dt);

    // Gravity
    velocity.y += gravity * dt;
    if (velocity.y > terminalVelocity) velocity.y = terminalVelocity;

    // Update position
    position += velocity * dt;

    // Ground collision
    if (position.y >= 500.0f) {
        position.y = 500.0f;
        velocity.y = 0.0f;
		jumpCount = 0; // reset jumps on landing
    // Simple ground collision
    float groundLevel = 600.0f - m_shape.getSize().x/2;
    if (position.y >= groundLevel) {
        position.y = groundLevel;
        velocity.y = 0;
        jumpCount = 0; // reset jumps on landing
    }

    m_shape.setPosition(position);
}
