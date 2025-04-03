#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.hpp"

#include "imgui.h"
#include "imgui-SFML.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 800)), "Test window");
	ImGui::SFML::Init(window);

	sf::CircleShape shape(200, 100);
	shape.setFillColor(sf::Color(0, 255, 255));
	shape.setPosition(sf::Vector2f(200, 200)); // Center circle

	GameObjects::Game& game = GameObjects::Game::getInstance();

	game.initialize(/*GameObjects::Difficulty::EASY*/);

	sf::Clock deltaClock;
	sf::Clock masterClock;

	deltaClock.start();
	masterClock.start();
	while (window.isOpen())
	{
		deltaClock.restart();
		const sf::Event UIevent();
		while (const std::optional event = window.pollEvent())
		{
			ImGui::SFML::ProcessEvent(window, *event);
			//Close window if X is clicked
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
		}

		game.deltaTime = deltaClock.getElapsedTime().asSeconds();
		game.elapsedTime = masterClock.getElapsedTime().asSeconds();
		game.update();

		//if (io.DeltaTime <= 0.0f) io.DeltaTime = 0.00001f;
		ImGui::SFML::Update(window, deltaClock.getElapsedTime());

		ImGui::Begin("Window title");
		ImGui::Text("Window text");
		ImGui::End();

		//main loop will go here
		//should primarily be used for high level function calls

		//render 
		window.clear(sf::Color(18, 33, 43)); // Color background
		window.draw(shape);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
