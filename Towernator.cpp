#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.hpp"

#include "imgui.h"
#include "imgui-SFML.h"


int main()
{
	//sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(sf::Vector2u(GameObjects::WINDOW_WIDTH, GameObjects::WINDOW_HEIGHT)), "Towernator Demo", sf::Style::Titlebar | sf::Style::Close);
	ImGui::SFML::Init(window);

	GameObjects::Game& game = GameObjects::Game::getInstance();
	game.window = &window;

	game.initialize(/*GameObjects::Difficulty::EASY*/);
	game.placeTestTower();

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

		//menu screen
		bool easyButtonClicked = false;
		bool mediumButtonClicked = false;
		bool hardButtonClicked = false;

		ImGui::Begin("Menu");
		if (ImGui::Button("EASY"))
		{
			std::cout << "The button has been clicked!\n"; //remove this later, used for debug
			easyButtonClicked = true;
		}
		ImGui::Text("Start game with easy difficulty");
		if (ImGui::Button("MEDIUM"))
		{
			std::cout << "This difficulty has not been implemented yet...\n"; //remove this later, used for debug
			mediumButtonClicked = true;
		}
		if (mediumButtonClicked)
		{
			ImGui::Begin("Information");
			ImGui::Text("This difficulty has not been implemented yet...");
			ImGui::End();
			mediumButtonClicked = false;
		}
		ImGui::Text("Start game with medium difficulty");
		if (ImGui::Button("HARD"))
		{
			std::cout << "This difficulty has not been implemented yet...\n"; //remove this later, used for debug
			hardButtonClicked = true;
		}
		if (hardButtonClicked)
		{
			ImGui::Begin("Information");
			ImGui::Text("This difficulty has not been implemented yet...");
			ImGui::End();
			hardButtonClicked = false;
		}
		ImGui::Text("Start game with hard difficulty (good luck)");
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
