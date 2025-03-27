#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.hpp"



int main()
{
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(sf::Vector2u(700, 700)), "Testing");

	GameObjects::Game& game = GameObjects::Game::getInstance();

	game.initialize(GameObjects::Difficulty::MEDIUM);

	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window->close();
			}
		}

		//main loop will go here
		//should primarily be used for high level function calls

		//render 

		window->clear();

		//draw stuff

		window->display();
	}

	delete window;
	return 0;
}
