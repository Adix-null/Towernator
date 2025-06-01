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

		renderEnemyHealthBars();
	}

	void GameObjects::Game::renderEnemyHealthBars()
	{
		if (!window)
			throw Exceptions::TowernatorException("Window pointer not initialized");

		for (const auto& enemyPtr : enemies)
		{
			const Enemy& enemy = *enemyPtr;

			auto [gamePos, angle] = interpolatePath(pathPoints, enemy.progressInPath);
			sf::Vector2f screenPos = GameToWindowCoords(gamePos, WINDOW_HEIGHT);

			const float barWidth = 80.0f;
			const float barHeight = 10.0f;
			const float verticalOffset = 75.0f;

			float healthRatio = static_cast<float>(enemy.health) / std::max(1, enemy.maxHealth);
			healthRatio = std::clamp(healthRatio, 0.0f, 1.0f);

			sf::RectangleShape bgBar({ barWidth, barHeight });
			bgBar.setFillColor(sf::Color(255, 255, 255));
			bgBar.setPosition({ screenPos.x - barWidth / 2.f, screenPos.y - verticalOffset });

			sf::RectangleShape fgBar({ barWidth * healthRatio, barHeight });
			fgBar.setFillColor(sf::Color(255, 0, 0));
			fgBar.setOutlineColor(sf::Color::White);
			fgBar.setOutlineThickness(4.0f);
			fgBar.setPosition({ screenPos.x - barWidth / 2.f, screenPos.y - verticalOffset });

			//window->draw(bgBar);
			window->draw(fgBar);
		}
	}



}