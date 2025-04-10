#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.hpp"

#include "imgui.h"
#include "imgui-SFML.h"


int main()
{
	//sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(GameObjects::WINDOW_WIDTH, GameObjects::WINDOW_HEIGHT)), "Test window", sf::Style::Titlebar | sf::Style::Close);
	ImGui::SFML::Init(window);

	GameObjects::Game& game = GameObjects::Game::getInstance();
	game.window = &window;

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
		window.clear(sf::Color(32, 32, 32));
		window.setFramerateLimit(120);

		game.deltaTime = std::max(deltaClock.getElapsedTime().asSeconds(), 1/120.0f);
		game.elapsedTime = masterClock.getElapsedTime().asSeconds();
		game.update();
		if (game.state == GameObjects::GameState::GAME_OVER)
		{
			break;
		}

		//if (io.DeltaTime <= 0.0f) io.DeltaTime = 0.00001f;
		ImGui::SFML::Update(window, deltaClock.getElapsedTime());

		ImGui::Begin("Window title");
		ImGui::Text("Window text");
		ImGui::End();

		//main loop will go here
		//should primarily be used for high level function calls

		//render 
		//GameObjects::Game::getInstance().drawGrid(32, 32);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
