#include "GameObjects.hpp"

namespace GameObjects
{
	void Game::togglePause(bool gamePaused)
	{
		paused = gamePaused;

		if (paused) {
			std::cout << "Game paused.\n";
		}
		else {
			std::cout << "Game resumed.\n";
		}
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
			loadTextureIntoBuffer("Saules_sprites/Towers/fast_tower_shot.gif");
			loadTextureIntoBuffer("Saules_sprites/Enemies/robot_enemy_blow.gif");
			loadTextureIntoBuffer("Saules_sprites/Maps/PauseScreen.png");
			state = GameState::ROUND_INIT;

			loadWaveDataFromFile("Wave_data/Waves.txt", Difficulty::EASY);
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
		if (paused) {
			if (textureCache.find("Saules_sprites/Maps/PauseScreen.png") == textureCache.end()) {
				throw Exceptions::TowernatorException("Required texture not found in cache");
			}

			renderBackground(textureCache.at("Saules_sprites/Maps/PauseScreen.png"));

			// Skip updating game state if paused
			return;
		}

		try {
			// Only handle logic-related exceptions here
			if (textureCache.find("Saules_sprites/Maps/map1_gp_complete.gif") == textureCache.end()) {
				throw Exceptions::TowernatorException("Required texture not found in cache");
			}

			renderBackground(textureCache.at("Saules_sprites/Maps/map1_gp_complete.gif"));

			processEnemyData();
			renderEnemyData();
			processTowerData();
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

	void GameObjects::Game::saveGame(const std::string& filename) {
		std::ofstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open save file for writing.\n";
			return;
		}

		file << "ROUND:" << roundNumber << "\n";
		file << "GOLD:" << gold << "\n";
		file << "HEALTH:" << centralFactoryHealth << "\n";
		file << "SCORE:" << score << "\n\n";

		for (const auto& tower : towers) {
			std::string type;
			if (tower->path.find("fast") != std::string::npos)
				type = "FAST";
			else if (tower->path.find("splash") != std::string::npos)
				type = "SPLASH";
			else
				type = "STREAM";

			file << "TOWER:" << type << " "
				<< tower->position.x << " "
				<< tower->position.y << " "
				<< tower->rotation << "\n";
		}

		std::cout << "Saved towers\n";

		for (const auto& enemy : enemies) {
			std::string type = enemy->getType();

			file << "ENEMY:" << type << " "
				<< enemy->health << " "
				<< enemy->progressInPath << "\n";
		}
		std::cout << "Game saved to " << filename << "\n";
		file.close();
	}

	void GameObjects::Game::loadGame(const std::string& filename) {

		towers.clear();
		enemies.clear();
		spawnQueue = std::queue<SpawnEvent>();

		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open save file for reading.\n";
			return;
		}

		std::string line;
		towers.clear();
		enemies.clear();

		while (std::getline(file, line)) {
			if (line.empty()) continue;

			std::istringstream iss(line);
			std::string label;
			std::getline(iss, label, ':');

			if (label == "ROUND") {
				iss >> roundNumber;
			}
			else if (label == "GOLD") {
				iss >> gold;
			}
			else if (label == "HEALTH") {
				iss >> centralFactoryHealth;
			}
			else if (label == "SCORE") {
				iss >> score;
			}
			else if (label == "TOWER") {
				std::string typeStr;
				float x, y, rotation;
				iss >> typeStr >> x >> y >> rotation;

				TowerType type = (typeStr == "FAST") ? TowerType::FAST :
					(typeStr == "SPLASH") ? TowerType::SPLASH :
					TowerType::STREAM;

				auto tower = TowerFactory::createTower(type, sf::Vector2f(x, y));
				tower->rotation = rotation;
				towers.push_back(std::move(tower));
			}
			else if (label == "ENEMY") {
				std::string typeStr;
				int health;
				float progress;
				iss >> typeStr >> health >> progress;
				std::cout << "Loading enemy of type: " << typeStr << "\n";

				EnemyType type =
					(typeStr.find("Runner") != std::string::npos) ? EnemyType::RUNNER :
					(typeStr.find("Walker") != std::string::npos) ? EnemyType::WALKER :
					(typeStr.find("Tank") != std::string::npos) ? EnemyType::TANK : EnemyType::BOSS;

				auto enemy = EnemyFactory::createEnemy(type);
				enemy->health = health;
				enemy->progressInPath = progress;
				enemies.push_back(std::move(enemy));
			}
		}
		file.close();
		std::cout << "Game loaded from " << filename << "\n";

		state = GameState::ROUND_ACTION;
	}


}
