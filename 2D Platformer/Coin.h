#pragma once
#include <SFML/Graphics.hpp>

class Coin {
public:
    Coin();
    Coin(float x, float y);

    void draw(sf::RenderWindow& win) const;
    void update(float dt);
    void collect();

    sf::FloatRect getBounds() const;
    bool isCollected() const { return collected; }

    bool collected = false;
    int value = 10;

private:
    sf::CircleShape shape;
    sf::Vector2f position;
    float bobTimer = 0.f;
};
