#pragma once
#include <SFML/Graphics.hpp>

class Platform {
public:
    Platform() = default;
    Platform(float x, float y, float w, float h);

    void draw(sf::RenderWindow& win) const;
    sf::FloatRect getBounds() const;
    sf::Vector2f getSize() const { return shape.getSize(); }

    // center position & size (accessible by Game)
    sf::Vector2f position;
    sf::Vector2f size;

private:
    sf::RectangleShape shape;
};
