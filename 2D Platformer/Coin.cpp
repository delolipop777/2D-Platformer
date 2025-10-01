#include "Coin.h"
#include <cmath>

Coin::Coin()
{
    shape.setRadius(12.f);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setFillColor(sf::Color(212, 175, 55));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(160, 120, 20));
    position = { 0.f, 0.f };
    shape.setPosition(position);
    value = 10;
    collected = false;
}

Coin::Coin(float x, float y)
{
    shape.setRadius(12.f);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setFillColor(sf::Color(212, 175, 55));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(160, 120, 20));
    position = { x, y };
    shape.setPosition(position);
    value = 10;
    collected = false;
}

void Coin::draw(sf::RenderWindow& win) const
{
    if (collected) return;
    win.draw(shape);

    // small shine
    sf::CircleShape shine(shape.getRadius() * 0.5f);
    shine.setOrigin(shine.getRadius(), shine.getRadius());
    shine.setFillColor(sf::Color(255, 255, 255, 180));
    shine.setPosition(shape.getPosition().x - 4.f, shape.getPosition().y - 6.f);
    win.draw(shine);
}

void Coin::update(float dt)
{
    if (collected) return;
    bobTimer += dt * 2.f;
    float bob = std::sin(bobTimer) * 6.f;
    shape.setPosition(position.x, position.y + bob);
}

void Coin::collect()
{
    collected = true;
    // could add sound/particles here
}

sf::FloatRect Coin::getBounds() const
{
    return shape.getGlobalBounds();
}
