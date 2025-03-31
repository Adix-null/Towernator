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
		if (type == EnemyType::RUNNER) return std::make_unique<Runner>();
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

	void Game::initialize(Difficulty dif)
	{
		switch (dif)
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
		}
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

	void Game::run()
	{

	}

	void Game::end()
	{

	}
}