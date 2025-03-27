#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <math.h>
#include "GameObjects.hpp"

namespace GameObjects
{
	float GameObjects::EnemyToughnessMultiplierFunction(int roundNumber)
	{
		return std::max(log(roundNumber) / log(5), 1.0);
	}
	void GameObjects::Game::togglePause(bool gamePaused)
	{

	}

	void GameObjects::Game::initialize(GameObjects::Difficulty dif)
	{

	}

	void GameObjects::Game::run()
	{

	}

	void GameObjects::Game::end()
	{

	}
}