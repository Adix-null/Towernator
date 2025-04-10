#include "GameObjects.hpp"

namespace GameObjects
{
	//info hardcoded for now
	void Game::loadWaveDataFromFile()
	{
		pathPoints.push_back({ 16, 2 }); //start
		pathPoints.push_back({ 2, 2 });
		pathPoints.push_back({ 2, 8 });
		pathPoints.push_back({ 15, 8 });
		pathPoints.push_back({ 15, 4 });
		pathPoints.push_back({ 4, 4 });
		pathPoints.push_back({ 4, 6 });
		pathPoints.push_back({ 12, 6 }); //end

		waves.resize(3);

		// Wave 1
		waves[0].push_back({ 5, EnemyType::RUNNER, 0.66f, 0 });
		waves[0].push_back({ 5, EnemyType::WALKER, 1.0f, 3 });

		// Wave 2
		waves[1].push_back({ 3, EnemyType::TANK, 2.0f, 5 });
		waves[1].push_back({ 5, EnemyType::WALKER, 0.4f, 5 });

		// Wave 3
		//waves[2].push_back({ 1, EnemyType::BOSS, 3.0f, 10 });
		//waves[2].push_back({ 15, EnemyType::RUNNER, 0.3f, 10000 });

		gold = 1000;
		centralFactoryHealth = 1300;
		startRoundDelay = 2;
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