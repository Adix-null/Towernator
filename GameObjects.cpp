#include "GameObjects.hpp"

namespace GameObjects
{
	void Game::togglePause(bool gamePaused)
	{
		// Implementation to be added
	}

	void Game::initialize()
	{
		try {
			GifAnimator a1;
			a1.load("Saules_sprites/Enemies/robot_enemy1");
			animators.push_back(a1);

			loadTextureIntoBuffer("Saules_sprites/Maps/map1_gp_complete.gif");
			loadTextureIntoBuffer("Saules_sprites/Enemies/robot_enemy1.gif");
			loadTextureIntoBuffer("Saules_sprites/Towers/fast_tower.gif");
			loadTextureIntoBuffer("Saules_sprites/Towers/damage_tower.gif");
			loadTextureIntoBuffer("Saules_sprites/Towers/splash_tower.gif");

			state = GameState::ROUND_INIT;

			loadWaveDataFromFile();
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("Game initialization failed: " + std::string(e.what()));
		}
		catch (...) {
			throw Exceptions::TowernatorException("Game initialization failed due to unknown error.");
		}
	}

	void Game::update()
	{
		try {
			// Only handle logic-related exceptions here
			if (textureCache.find("Saules_sprites/Maps/map1_gp_complete.gif") == textureCache.end()) {
				throw Exceptions::TowernatorException("Required texture not found in cache");
			}

			renderBackground(textureCache.at("Saules_sprites/Maps/map1_gp_complete.gif"));

			processEnemyData();
			processTowerData();
			renderEnemyData();
			renderTowerData();

			for (auto& anim : animators) {
				anim.update(deltaTime);
			}

			if (spawnQueue.empty() && roundNumber <= waves.size()) {
				state = GameState::ROUND_INIT;
			}

			if ((roundNumber > waves.size() && spawnQueue.empty() && enemies.empty()) || centralFactoryHealth <= 0) {
				state = GameState::GAME_OVER;
			}

			if (state == GameState::ROUND_INIT) {
				if (roundNumber <= 0 || roundNumber > waves.size()) {
					throw Exceptions::TowernatorException("Invalid round number: " + std::to_string(roundNumber));
				}

				loadRoundWaveData(std::min(roundNumber, static_cast<int>(waves.size())) - 1);

				state = GameState::ROUND_ACTION;
				roundNumber++;
			}

			if (state == GameState::GAME_OVER) {
				end();
			}

			if (state == GameState::ROUND_ACTION) {
				while (!spawnQueue.empty() && spawnQueue.front().spawnTime <= elapsedTime) {
					SpawnEvent& event = spawnQueue.front();
					spawnEnemy(event.type);
					spawnQueue.pop();
				}
			}
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("Game update failed: " + std::string(e.what()));
		}
	}

	void Game::end()
	{
		std::cout << "Game over! Rounds survived: " << roundNumber << "\n";
	}
}
