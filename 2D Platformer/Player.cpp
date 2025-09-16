#include "Player.h"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>

Player::Player()
    : health(100), score(0), position(400.0f, 500.0f), velocity(0.0f, 0.0f)
{
    // Appearance
    m_shape.setSize(sf::Vector2f(50.0f, 50.0f));
    m_shape.setFillColor(sf::Color(50, 150, 255)); // nicer blue
    m_shape.setOutlineThickness(3.0f);
    m_shape.setOutlineColor(sf::Color::White);
    m_shape.setOrigin(m_shape.getSize() / 2.f); // center origin
    m_shape.setPosition(position);

    // Physics params
    maxSpeed = 250.0f;
    accel = 800.0f;
    decel = 900.0f;
    jumpStrength = -450.0f;
    gravity = 1000.0f;
    terminalVelocity = 650.0f;

    // Jumping
    jumpCount = 0;
    maxJumps = 2; // allows double jump
}

void Player::handleInput(float dt) {
    // Horizontal movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) velocity.x -= accel * dt;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) velocity.x += accel * dt;
    else {
        if (velocity.x > 0) { velocity.x -= decel * dt; if (velocity.x < 0) velocity.x = 0; }
        else if (velocity.x < 0) { velocity.x += decel * dt; if (velocity.x > 0) velocity.x = 0; }
    }

    velocity.x = std::clamp(velocity.x, -maxSpeed, maxSpeed);

    // Jump: only on key press (rising edge)
    bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    if (jumpPressed && !jumpKeyPressedLastFrame && jumpCount < maxJumps) {
        jump();
    }

    jumpKeyPressedLastFrame = jumpPressed; // update state for next frame
}


void Player::jump() {
    velocity.y = jumpStrength;
    jumpCount++;
}

void Player::update(float dt) {
    handleInput(dt);

    // Gravity
    velocity.y += gravity * dt;
    if (velocity.y > terminalVelocity) velocity.y = terminalVelocity;

    // Apply velocity to position
    position += velocity * dt;

    // Simple ground collision
    float groundLevel = 500.0f;
    if (position.y >= groundLevel) {
        position.y = groundLevel;
        velocity.y = 0;
        jumpCount = 0; // reset jumps on landing
    }

    m_shape.setPosition(position);
}
