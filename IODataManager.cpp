#include "GameObjects.hpp"
namespace GameObjects
{
    void Game::loadWaveDataFromFile(const std::string& filepath, Difficulty dif)
    {
        pathPoints.clear();
        waves.clear();
        gold = 0;
        centralFactoryHealth = 0;
        startRoundDelay = 0.0f;

        std::ifstream file(filepath);
        if (!file.is_open()) {
            throw Exceptions::TowernatorException("Could not open wave_data.txt file");
        }

        std::string line;
        std::string section = "";
        std::string currentDifficulty = "";

        // Convert difficulty enum to string for comparison
        std::string targetDifficulty;
        switch (dif) {
        case Difficulty::EASY: targetDifficulty = "EASY"; break;
        case Difficulty::MEDIUM: targetDifficulty = "MEDIUM"; break;
        case Difficulty::HARD: targetDifficulty = "HARD"; break;
        case Difficulty::INFINITE: targetDifficulty = "INFINITE"; break;
        }

        while (std::getline(file, line)) {
            // Skip comments / empty lines
            if (line.empty() || line[0] == '#') {
                continue;
            }

            if (line[0] == '[' && line.back() == ']') {
                section = line.substr(1, line.length() - 2);

                if (section == "DIFFICULTY_EASY" || section == "DIFFICULTY_MEDIUM" ||
                    section == "DIFFICULTY_HARD" || section == "DIFFICULTY_INFINITE") {
                    currentDifficulty = section.substr(11);
                }
                else {
                    currentDifficulty = "";
                }
                continue;
            }

            std::istringstream iss(line);

            if (section == "PATH") {
                int x, y;
                if (iss >> x >> y) {
                    pathPoints.push_back({ x, y });
                }
            }
            else if (section == "GAME_SETTINGS" ||
                (!currentDifficulty.empty() && currentDifficulty == targetDifficulty)) {
                std::string key;
                if (iss >> key) {
                    if (key == "gold") {
                        iss >> gold;
                    }
                    else if (key == "centralFactoryHealth") {
                        iss >> centralFactoryHealth;
                    }
                    else if (key == "startRoundDelay") {
                        iss >> startRoundDelay;
                    }
                    else if (key == "waveCount") {
                        int waveCount;
                        iss >> waveCount;
                        waves.resize(waveCount);
                    }
                }
            }
            else if (section.substr(0, 4) == "WAVE") {
                int waveIndex;
                try {
                    waveIndex = std::stoi(section.substr(4)) - 1;
                }
                catch (const std::invalid_argument&) {
                    throw Exceptions::TowernatorException("Invalid wave section format: " + section);
                }
                catch (const std::out_of_range&) {
                    throw Exceptions::TowernatorException("Wave number out of range: " + section);
                }

                if (waveIndex < 0 || waveIndex >= waves.size()) {
                    throw Exceptions::TowernatorException("Wave index " + std::to_string(waveIndex + 1) +
                        " is out of bounds. Expected 1-" + std::to_string(waves.size()));
                }
                int count;
                std::string enemyTypeStr;
                float spawnDelay;
                int healthBoost;

                if (!(iss >> count >> enemyTypeStr >> spawnDelay >> healthBoost)) {
                    throw Exceptions::TowernatorException("Invalid wave data format in " + section +
                        ". Expected: count enemyType spawnDelay healthBoost");
                }

                if (count <= 0) {
                    throw Exceptions::TowernatorException("Enemy count must be positive in " + section);
                }

                if (spawnDelay < 0) {
                    throw Exceptions::TowernatorException("Spawn delay cannot be negative in " + section);
                }
                EnemyType enemyType;
                if (enemyTypeStr == "RUNNER") {
                    enemyType = EnemyType::RUNNER;
                }
                else if (enemyTypeStr == "WALKER") {
                    enemyType = EnemyType::WALKER;
                }
                else if (enemyTypeStr == "TANK") {
                    enemyType = EnemyType::TANK;
                }
                else if (enemyTypeStr == "BOSS") {
                    enemyType = EnemyType::BOSS;
                }
                else {
                    throw Exceptions::TowernatorException("Unknown enemy type: " + enemyTypeStr);
                }

                waves[waveIndex].push_back({ count, enemyType, spawnDelay, healthBoost });
            }
        }

        if (pathPoints.empty()) {
            throw Exceptions::TowernatorException("No path points found in wave_data.txt");
        }

        bool hasWaveData = false;
        for (const auto& wave : waves) {
            if (!wave.empty()) {
                hasWaveData = true;
                break;
            }
        }

        if (!hasWaveData) {
            throw Exceptions::TowernatorException("No wave data found in wave_data.txt");
        }

        file.close();
    }

	void Game::loadRoundWaveData(int waveNum)
	{
		spawnQueue = {};
		float currentTime = elapsedTime + startRoundDelay;
		float prevEnd = currentTime;

		for (int i = 0; i < waves[waveNum].size(); i++)
		{
			float waveStartTime = prevEnd + waves[waveNum][i].startWaveDelay;

			for (int j = 0; j < waves[waveNum][i].enemyCount; j++)
			{
				spawnQueue.push({ waveStartTime + j * waves[waveNum][i].instanceDelay, waves[waveNum][i].type });
				if (j == waves[waveNum][i].enemyCount - 1)
				{
					prevEnd = waveStartTime + j * waves[waveNum][i].instanceDelay;
				}
				std::cout << waveStartTime + j * waves[waveNum][i].instanceDelay << "s t: " << static_cast<int>(waves[waveNum][i].type) << "\n";
			}
		}
	}

	void Game::placeTestTower()
	{
		spawnTower(TowerType::FAST, sf::Vector2f(3.0f, 5.0f));
		spawnTower(TowerType::STREAM, sf::Vector2f(14.0f, 3.0f));
		spawnTower(TowerType::SPLASH, sf::Vector2f(8.0f, 3.0f));
		//std::cout << "Placed FAST tower at (" << pos.x << ", " << pos.y << ")\n";
	}

	//difficulty will be handled later

		/*switch (dif)
		{
		case Difficulty::EASY:
		{
			gold = 1000;
			centralFactoryHealth = 300;
			startRoundDelay = 10;


			break;
		}
		case Difficulty::MEDIUM:
		{
			gold = 500;
			centralFactoryHealth = 200;
			startRoundDelay = 7;


			break;
		};
		case Difficulty::HARD:
		{
			gold = 200;
			centralFactoryHealth = 150;
			startRoundDelay = 4;


			break;
		};
		case Difficulty::INFINTE:
		{
			gold = 300;
			centralFactoryHealth = 300;
			startRoundDelay = 5;


			break;
		};
		}*/
}