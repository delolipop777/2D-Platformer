#include <SFML\Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Platformer");

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				window.close();
			}
		}
	}
}