#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <math.h>
#include "GameObjects.hpp"

namespace GameObjects
{
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

	void Game::run()
	{

	}

	void Game::end()
	{

	}
}