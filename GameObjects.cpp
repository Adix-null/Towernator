#include "GameObjects.hpp"

namespace GameObjects
{
	void Game::togglePause(bool gamePaused)
	{

	}

	void Game::initialize(/*Difficulty dif*/)
	{
		loadTextureIntoBuffer("Saules_sprites/Maps/map1_gp_complete.gif");
		loadTextureIntoBuffer("Saules_sprites/Enemies/robot_enemy1.gif");
		loadTextureIntoBuffer("Saules_sprites/Towers/fast_tower.gif");

		state = GameState::ROUND_INIT;

		loadWaveDataFromFile();
	}

	void Game::update()
	{
		renderBackground(textureCache.at("Saules_sprites/Maps/map1_gp_complete.gif"));
		processEnemyData();
		processTowerData();
		renderTowerData();

		if (spawnQueue.empty())
		{
			state = GameState::ROUND_INIT;
		}
		if ((roundNumber > waves.size() && spawnQueue.empty() && enemies.empty()) || centralFactoryHealth <= 0)
		{
			state = GameState::GAME_OVER;
		}
		if (state == GameState::ROUND_INIT)
		{
			loadRoundWaveData(std::min(roundNumber, static_cast<int>(waves.size())) - 1);
			state = GameState::ROUND_ACTION;
			roundNumber++;
		}
		if (state == GameState::GAME_OVER)
		{
			end();
		}


		if (state == GameState::ROUND_ACTION)
		{
			while (!(spawnQueue.empty()) && spawnQueue.front().spawnTime <= elapsedTime)
			{
				SpawnEvent& event = spawnQueue.front();
				spawnEnemy(event.type);
				spawnQueue.pop();
			}
		}
	}

	void Game::processEnemyData()
	{
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			auto& enemy = **it;
			auto transformationResult = GameObjects::interpolatePath(pathPoints, enemy.progressInPath);
			enemy.progressInPath += enemy.speed * deltaTime;
			renderImage(textureCache.at("Saules_sprites/Enemies/robot_enemy1.gif"), GameToWindowCoords(transformationResult.first, WINDOW_HEIGHT), transformationResult.second);

			if (enemy.progressInPath >= 1.0f)
			{
				centralFactoryHealth -= enemy.damage;
				std::cout << "hit! Health = " << centralFactoryHealth << "\n";
				it = enemies.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void Game::processTowerData()
	{
		static float lastPrintTime = 0.0f;
		float printInterval = 1.5f; // seconds between prints

		for (const auto& tower : towers)
		{
			const sf::Vector2f& towerPos = tower->position;

			Enemy* bestTarget = nullptr;
			float maxProgress = -1.0f;

			for (const auto& enemy : enemies)
			{
				sf::Vector2f enemyPos = interpolatePosition(pathPoints, enemy->progressInPath);

				if (dist(towerPos, enemyPos) <= tower->range * 32.0f)
				{
					if (enemy->progressInPath > maxProgress)
					{
						maxProgress = enemy->progressInPath;
						bestTarget = enemy.get();
					}
				}
			}

			if (bestTarget)
			{
				sf::Vector2f enemyPos = interpolatePosition(pathPoints, bestTarget->progressInPath);
				sf::Vector2f direction = enemyPos - towerPos;
				float angleRadians = std::atan2(direction.y, direction.x);
				float angleDegrees = angleRadians * 180.0f / 3.14159f;

				tower->rotation = angleDegrees;

				if (elapsedTime - lastPrintTime >= printInterval)
				{
					std::cout << "Tower at (" << towerPos.x << ", " << towerPos.y << ") targeting enemy at ("
						<< enemyPos.x << ", " << enemyPos.y << "), Rotation: " << angleDegrees << "°\n";

					std::cout << "Checking distance from tower at " << towerPos.x << ", " << towerPos.y
						<< " to enemy at " << enemyPos.x << ", " << enemyPos.y
						<< " = " << dist(towerPos, enemyPos) << "\n";

					lastPrintTime = elapsedTime;
				}
			}
		}
	}

	void Game::renderTowerData()
	{
		for (const auto& tower : towers)
		{
			renderImage(
				textureCache.at("Saules_sprites/Towers/fast_tower.gif"), // Pick texture based on tower type if needed
				tower->position,
				tower->rotation
			);
		}
	}


	void Game::end()
	{
		std::cout << "Game over! Score: " << score << "\n";
		return;
	}
}