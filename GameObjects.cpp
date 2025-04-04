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
		if (type == EnemyType::RUNNER) return std::make_unique<Runner>(50, 30, 10, 40, new std::string("ff0000"));
		if (type == EnemyType::NORMAL) return std::make_unique<Normal>();
		if (type == EnemyType::TANK) return std::make_unique<Tank>();
		if (type == EnemyType::BOSS) return std::make_unique<Boss>();
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

	void Game::spawnEnemy(EnemyType type)
	{
		auto enemy = EnemyFactory::createEnemy(type);
		if (enemy)
		{
			this->enemies.push_back(std::move(enemy));
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

	//majority of logic goes here
	void Game::update(float deltaTime, float elapsedTime)
	{
		//TODO: enemy spawning according to elapsed time with Game::spawnEnemy and waves

		//tower handling
		for (const auto& tower : towers)
		{

		}

		for (const auto& enemy : enemies)
		{
			enemy->progressInPath += enemy->speed * deltaTime;

			if (enemy->progressInPath >= 1.0f)
				this->centralFactoryHealth -= enemy->damage;

			//TODO: render enemy sprites
		}

		if (this->centralFactoryHealth == 0)
			Game::end();



	}

	void Game::end()
	{

	}
}