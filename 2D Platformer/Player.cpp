#include "Player.h"
#include <SFML/Window/Keyboard.hpp>

Player::Player()
{
    shape.setSize(sf::Vector2f(48.f, 64.f));
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setFillColor(sf::Color(50, 120, 200));

    maxSpeed = 260.f;
    accel = 1600.f;
    decel = 2000.f;
    jumpStrength = -520.f;
    gravity = 1600.f;
    terminalVelocity = 1100.f;

    position = sf::Vector2f(100.f, 600.f);
    velocity = sf::Vector2f(0.f, 0.f);
    jumpCount = 0;
    onGround = false;

    shape.setPosition(position);
}

void Player::resetTo(const sf::Vector2f& pos)
{
    position = pos;
    velocity = { 0.f, 0.f };
    jumpCount = 0;
    onGround = false;
    shape.setPosition(position);
}

void Player::handleInput(float dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= accel * dt;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += accel * dt;
    }
    else {
        // decelerate smoothly
        if (velocity.x > 0.f) {
            velocity.x -= decel * dt;
            if (velocity.x < 0.f) velocity.x = 0.f;
        }
        else if (velocity.x < 0.f) {
            velocity.x += decel * dt;
            if (velocity.x > 0.f) velocity.x = 0.f;
        }
    }

    if (velocity.x > maxSpeed) velocity.x = maxSpeed;
    if (velocity.x < -maxSpeed) velocity.x = -maxSpeed;

    static bool jumpKeyDown = false;
    bool space = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
    if (space && !jumpKeyDown && jumpCount < 2) {
        velocity.y = jumpStrength;
        jumpCount++;
    }
    jumpKeyDown = space;
}

void Player::update(float dt)
{
    handleInput(dt);

    // gravity
    velocity.y += gravity * dt;
    if (velocity.y > terminalVelocity) velocity.y = terminalVelocity;

    // update position
    position += velocity * dt;

    // respawn if falling too far
    if (position.y > 3000.f) resetTo({ 100.f, 600.f });

    shape.setPosition(position);
}

void Player::draw(sf::RenderWindow& win) const
{
    win.draw(shape);
    // simple shadow
    sf::CircleShape shadow(22.f);
    shadow.setOrigin(shadow.getRadius(), shadow.getRadius());
    shadow.setPosition(position.x, position.y + shape.getSize().y / 2.f + 8.f);
    shadow.setFillColor(sf::Color(0, 0, 0, 100));
    win.draw(shadow);
}

void Player::setPosition(const sf::Vector2f& pos)
{
    position = pos;
    shape.setPosition(pos);
}

sf::FloatRect Player::getBounds() const
{
    return shape.getGlobalBounds();
}

sf::Vector2f Player::getSize() const
{
    return shape.getSize();
}
