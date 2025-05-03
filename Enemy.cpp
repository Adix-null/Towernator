#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Enemy> EnemyFactory::createEnemy(const EnemyType& type)
	{
		if (type == EnemyType::RUNNER) return std::make_unique<Runner>(50, 30, 0.15f, 40, std::string("ff0000"));
		if (type == EnemyType::WALKER) return std::make_unique<Walker>(50, 30, 0.05f, 40, std::string("not implmented"));
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

	void Game::processEnemyData()
	{
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			auto& enemy = **it;
			enemy.progressInPath += enemy.speed * deltaTime;

			if (enemy.progressInPath >= 1.0f)
			{
				centralFactoryHealth -= enemy.damage;
				std::cout << "hit! Health = " << centralFactoryHealth << "\n";
				it = enemies.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void Game::renderEnemyData()
	{
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			auto& enemy = **it;
			auto transformationResult = GameObjects::interpolatePath(pathPoints, enemy.progressInPath);
			renderImage(textureCache.at("Saules_sprites/Enemies/robot_enemy1.gif"), GameToWindowCoords(transformationResult.first, WINDOW_HEIGHT), transformationResult.second);
		}
	}
}