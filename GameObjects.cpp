#include "GameObjects.hpp"

namespace GameObjects
{
	void Game::togglePause(bool gamePaused)
	{

	}

	void Game::initialize(/*Difficulty dif*/)
	{
		GifAnimator a1;
		a1.load("Saules_sprites/Enemies/robot_enemy1");
		animators.push_back(a1);

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
		renderEnemyData();
		renderTowerData();

		for (auto& anim : animators)
			anim.update(deltaTime);

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
			//switch (enemy.)

			animators[0].render(GameToWindowCoords(transformationResult.first, WINDOW_HEIGHT), transformationResult.second);

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
		for (const auto& tower : towers)
		{
			;
		}
	}


	void Game::end()
	{
		std::cout << "Game over! Score: " << score << "\n";
		return;
	}
}