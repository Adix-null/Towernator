#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Enemy> EnemyFactory::createEnemy(const EnemyType& type)
	{
		if (type == EnemyType::RUNNER) return std::make_unique<Runner>(50, 30, 0.15f, 40, std::string("ff0000"));
		if (type == EnemyType::WALKER) return std::make_unique<Walker>(50, 30, 0.05f, 40, std::string("00ff00"));
		if (type == EnemyType::TANK) return std::make_unique<Tank>(50, 30, 0.03f, 10, std::string("0000ff"));
		if (type == EnemyType::BOSS) return std::make_unique<Boss>(50, 30, 0.02f, 50, std::string("000000"));
		return nullptr;
	}

	void Game::spawnEnemy(EnemyType type)
	{
		auto enemy = EnemyFactory::createEnemy(type);
		if (enemy)
		{
			enemies.push_back(std::move(enemy));
			std::cout << "Spawned " << static_cast<int>(type) << " enemy\n";
		}
	}
}