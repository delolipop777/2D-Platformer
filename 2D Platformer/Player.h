#pragma once
#include <SFML/Graphics.hpp>

const float GRAVITY = 9.8f;
const float friction = 0.9f;
class Player
{
private:
	int health;
	int score;
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::RectangleShape m_shape;
	int jumpCount = 0;
	float maxSpeed;    // max horizontal speed
	float accel;          // horizontal acceleration
	float decel;         // horizontal deceleration
	float jumpStrength;  // upward impulse
	float gravity;        // pixels/s²
	float terminalVelocity;
public:
	Player();
	int getHealth() const { return health; }
	int getScore() const { return score; }
	void takeDamage(int damage) { health -= damage; if (health < 0) health = 0; }
	void addScore(int points) { score += points; }
	void update(float dt);
	void draw(sf::RenderWindow& window) { window.draw(m_shape); }
	void handleInput(float dt);
};

