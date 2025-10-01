#include "Platform.h"

Platform::Platform(float x, float y, float w, float h)
{
    // store center position
    position = { x + w / 2.f, y + h / 2.f };
    size = { w, h };
    shape.setSize(size);
    shape.setOrigin(size / 2.f);
    shape.setPosition(position);
    shape.setFillColor(sf::Color(120, 80, 40));
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color(80, 50, 20));
}

void Platform::draw(sf::RenderWindow& win) const
{
    win.draw(shape);
}

sf::FloatRect Platform::getBounds() const
{
    return shape.getGlobalBounds();
}
