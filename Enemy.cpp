#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Enemy> EnemyFactory::createEnemy(const EnemyType& type)
	{
		try {
			//Enemy(int hlt, int dmg, float spd, int rew, const std::string & path)
			if (type == EnemyType::RUNNER) return std::make_unique<Runner>(50, 30, 0.15f, 40, std::string("ff0000"));
			if (type == EnemyType::WALKER) return std::make_unique<Walker>(75, 30, 0.08f, 40, std::string("not implmented"));
			if (type == EnemyType::TANK) return std::make_unique<Tank>(150, 30, 0.06f, 10, std::string("0000ff"));
			if (type == EnemyType::BOSS) return std::make_unique<Boss>(300, 30, 0.03f, 50, std::string("000000"));

			throw Exceptions::TowernatorException("Unknown enemy type passed to createEnemy()");
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("Failed to create enemy: " + std::string(e.what()));
		}
	}

	void Game::spawnEnemy(EnemyType type)
	{
		auto enemy = EnemyFactory::createEnemy(type);
		if (!enemy)
			throw Exceptions::TowernatorException("createEnemy returned nullptr");

		enemies.push_back(std::move(enemy));
		std::cout << "Spawned " << static_cast<int>(type) << " enemy\n";
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
				std::cout << "hit! factory health = " << centralFactoryHealth << "\n";
				it = enemies.erase(it);
				break;
			}
			else if (enemy.health <= 0)
			{
				gold += enemy.reward;
				std::cout << "killed! gold = " << gold << "\n";
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
		if (animators.empty())
			throw Exceptions::TowernatorException("No animators loaded before rendering");

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			auto& enemy = **it;
			auto transformationResult = GameObjects::interpolatePath(pathPoints, enemy.progressInPath);
			animators[0].render(GameToWindowCoords(transformationResult.first, WINDOW_HEIGHT), transformationResult.second);

			if (enemy.hitDecal) {
				enemy.hitDecal->update(deltaTime);
				enemy.hitDecalElapsed += deltaTime;
				if (enemy.hitDecal->currentFrame >= enemy.hitDecal->frames.size() || enemy.hitDecalElapsed >= decalTime) {
					enemy.hitDecal.reset();
					enemy.hitDecalElapsed = 0.0f;
				}
			}
		}
	}
}