#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameObjects.hpp"

#include "imgui.h"
#include "imgui-SFML.h"



int main()
{
	//sf::RenderWindow window2(sf::VideoMode(sf::Vector2u(GameObjects::WINDOW_WIDTH, GameObjects::WINDOW_HEIGHT)), "Towernator Demo", sf::Style::Titlebar | sf::Style::Close);
	//while (window2.isOpen())
	//{
	//	const sf::Event UIevent();
	//	while (const std::optional event = window2.pollEvent())
	//	{
	//		ImGui::SFML::ProcessEvent(window2, *event);
	//		//Close window if X is clicked
	//		if (event->is<sf::Event::Closed>())
	//		{
	//			window2.close();
	//		}
	//	}
	//	window2.clear(sf::Color::Black);

	//	float barWidth = 80.0f;
	//	float barHeight = 10.0f;
	//	float healthRatio = 0.5f; // test value

	//	sf::RectangleShape bgBar({ barWidth, barHeight });
	//	bgBar.setFillColor(sf::Color(255, 0, 0, 255));
	//	bgBar.setPosition(sf::Vector2f(100, 100));

	//	sf::RectangleShape fgBar({ barWidth * healthRatio, barHeight });
	//	fgBar.setFillColor(sf::Color(255, 255, 255, 255));
	//	fgBar.setPosition(sf::Vector2f(100, 100));

	//	window2.draw(bgBar);
	//	window2.draw(fgBar);

	//	window2.display();
	//}

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

		game.deltaTime = std::max(deltaClock.getElapsedTime().asSeconds(), 1 / 120.0f);
		game.elapsedTime = masterClock.getElapsedTime().asSeconds();

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

		if (ImGui::Button(game.paused ? "Resume" : "Pause"))
		{
			game.togglePause(!game.paused);
		}

		if (ImGui::Button("Save Game"))
		{
			game.saveGame("savefile.txt");
			std::cout << "Game saved.\n";
		}

		if (ImGui::Button("Load Game"))
		{
			game.loadGame("savefile.txt");
			std::cout << "Game loaded.\n";
		}

		ImGui::End();
		ImGui::SFML::Render(window);

		game.update();
		if (game.state == GameObjects::GameState::GAME_OVER)
		{
			break;
		}

		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}
