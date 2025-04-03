#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <math.h>
#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Enemy> EnemyFactory::createEnemy(const EnemyType& type)
	{
		//TODO: file loading
		if (type == EnemyType::RUNNER) return std::make_unique<Runner>(50, 30, 10, 40, std::string("ff0000"));
		if (type == EnemyType::NORMAL) return std::make_unique<Walker>(50, 30, 10, 40, std::string("ff0000"));
		if (type == EnemyType::TANK) return std::make_unique<Tank>(50, 30, 10, 40, std::string("ff0000"));
		if (type == EnemyType::BOSS) return std::make_unique<Boss>(50, 30, 10, 40, std::string("ff0000"));
		return nullptr;
	}

	std::unique_ptr<Tower> TowerFactory::createTower(const TowerType& type)
	{
		if (type == TowerType::FAST) return std::make_unique<Fast>();
		if (type == TowerType::SPLASH) return std::make_unique<Splash>();
		if (type == TowerType::LASER) return std::make_unique<Laser>();
		return nullptr;
	}

	float Game::enemyToughnessMultiplierFunction(int roundNumber)
	{
		return std::max(log(roundNumber) / log(5), 1.0);
	}
	void Game::togglePause(bool gamePaused)
	{

	}

	void Game::initialize(/*Difficulty dif*/)
	{
		//difficulty will be handled later
		/*switch (dif)
		{
		case Difficulty::EASY:
		{
			this->gold = 1000;
			this->centralFactoryHealth = 300;
			this->startRoundDelay = 10;


			break;
		}
		case Difficulty::MEDIUM:
		{
			this->gold = 500;
			this->centralFactoryHealth = 200;
			this->startRoundDelay = 7;


			break;
		};
		case Difficulty::HARD:
		{
			this->gold = 200;
			this->centralFactoryHealth = 150;
			this->startRoundDelay = 4;


			break;
		};
		case Difficulty::INFINTE:
		{
			this->gold = 300;
			this->centralFactoryHealth = 300;
			this->startRoundDelay = 5;


			break;
		};
		}*/
	}

	//hardcoded for now
	void Game::loadWaveDataFromFile()
	{
		this->waves.resize(3);

		// Wave 1
		this->waves[0].push_back({ 5, EnemyType::RUNNER, 0.66f, 0 });
		this->waves[0].push_back({ 5, EnemyType::NORMAL, 1.0f, 3 });

		// Wave 2
		this->waves[1].push_back({ 4, EnemyType::TANK, 2.0f, 5 });
		this->waves[1].push_back({ 6, EnemyType::NORMAL, 0.4f, 5 });

		// Wave 3
		this->waves[2].push_back({ 3, EnemyType::BOSS, 3.0f, 10 });
		this->waves[2].push_back({ 7, EnemyType::RUNNER, 0.3f, 10 });

		this->gold = 1000;
		this->centralFactoryHealth = 300;
		this->startRoundDelay = 10;
	}

	void Game::loadWaveData(int waveNum)
	{
		this->spawnQueue = {};
		float currentTime = elapsedTime;

		for (const auto& wave : this->waves[waveNum])
		{
			float waveStartTime = currentTime + wave.startWaveDelay;

			for (int i = 0; i < wave.enemyCount; ++i)
			{
				this->spawnQueue.push({ waveStartTime + i * wave.instanceDelay, wave.type });
			}
		}
	}

	void Game::spawnEnemy(EnemyType type)
	{
		auto enemy = EnemyFactory::createEnemy(type);
		if (enemy)
		{
			this->enemies.push_back(std::move(enemy));
			std::cout << "Spawned " << static_cast<int>(type) << " enemy\n";
		}
	}
	void Game::spawnTower(TowerType type)
	{
		auto tower = TowerFactory::createTower(type);
		if (tower)
		{
			this->towers.push_back(std::move(tower));
		}
	}

	void Game::update()
	{
		processEnemyData();
		processTowerData();

		loadWaveDataFromFile();
		loadWaveData(this->roundNumber);

		while (!(this->spawnQueue.empty()) && this->spawnQueue.front().spawnTime <= elapsedTime)
		{

			SpawnEvent& event = this->spawnQueue.front();
			spawnEnemy(event.type);
			this->spawnQueue.pop();
		}

		if (this->centralFactoryHealth == 0)
			Game::end();
	}

	void Game::processEnemyData()
	{
		for (const auto& enemy : this->enemies)
		{
			enemy->progressInPath += enemy->speed * this->deltaTime;

			if (enemy->progressInPath >= 1.0f)
				this->centralFactoryHealth -= enemy->damage;
		}
	}

	void Game::processTowerData()
	{
		for (const auto& tower : towers)
		{

		}
	}

	void Game::end()
	{

	}
}