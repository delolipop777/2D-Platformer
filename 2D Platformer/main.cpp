#include <SFML\Graphics.hpp>
#include "Player.h"


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Platformer");

	Player player;
	
	sf::Clock clock;
	while (window.isOpen()) {
		float dt = clock.restart().asSeconds();
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear(sf::Color::White);
		player.update(dt);
		player.draw(window);
		window.display();
	}
}