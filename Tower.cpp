#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Tower> TowerFactory::createTower(const TowerType& type, const sf::Vector2f& position)
	{
		try {
			if (type == TowerType::FAST) return std::make_unique<Fast>(100, 10, 0.25f, 3.0f, "Saules_sprites/Towers/fast_tower.gif", position);
			if (type == TowerType::SPLASH) return std::make_unique<Splash>(200, 50, 0.75f, 2.5f, "Saules_sprites/Towers/splash_tower.gif", position);
			if (type == TowerType::STREAM) return std::make_unique<Stream>(150, 2, 0.05f, 4.0f, "Saules_sprites/Towers/damage_tower.gif", position);

			throw Exceptions::TowernatorException("Unknown tower type passed to createTower()");
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("Failed to create tower: " + std::string(e.what()));
		}
	}

	void Game::spawnTower(TowerType type, const sf::Vector2f& pos)
	{
		auto tower = TowerFactory::createTower(type, pos);
		if (!tower)
			throw Exceptions::TowernatorException("createTower returned nullptr");

		towers.push_back(std::move(tower));
	}

	void Game::processTowerData()
	{
		try {
			static float lastPrintTime = 0.0f;
			float printInterval = 1.5f; // seconds between prints

			for (const auto& tower : towers)
			{
				const sf::Vector2f& towerPos = tower->position;

				Enemy* bestTarget = nullptr;
				float maxProgress = -1.0f;

				for (const auto& enemy : enemies)
				{
					sf::Vector2f enemyPos = interpolatePosition(pathPoints, enemy->progressInPath);

					if (dist(towerPos, enemyPos) <= tower->range)
					{
						if (enemy->progressInPath > maxProgress)
						{
							maxProgress = enemy->progressInPath;
							bestTarget = enemy.get();
						}
					}
				}

				if (bestTarget)
				{
					sf::Vector2f enemyPos = interpolatePosition(pathPoints, bestTarget->progressInPath);
					sf::Vector2f direction = enemyPos - towerPos;
					float angleRadians = std::atan2(direction.y, direction.x);
					float angleDegrees = angleRadians * 180.0f / 3.14159f;

					tower->rotation = angleDegrees - 90.0f;

					(*tower).scanEnemies(bestTarget, deltaTime, tower->rateOfFire, tower->damagePerBullet);

					if (bestTarget->hitDecal) {
						bestTarget->hitDecal->render(
							GameToWindowCoords(enemyPos, WINDOW_HEIGHT),
							0.0f
						);
					}

					if (elapsedTime - lastPrintTime >= printInterval)
					{
						try {
							std::cout << "Tower at (" << towerPos.x << ", " << towerPos.y << ") targeting enemy at ("
								<< enemyPos.x << ", " << enemyPos.y << "), Rotation: " << angleDegrees << "deg\n";

							std::cout << "Checking distance from tower at " << towerPos.x << ", " << towerPos.y
								<< " to enemy at " << enemyPos.x << ", " << enemyPos.y
								<< " = " << dist(towerPos, enemyPos) << "\n";

							lastPrintTime = elapsedTime;
						}
						catch (const std::exception& e) {
							throw Exceptions::TowernatorException("Failed to print tower data: " + std::string(e.what()));
						}
					}
				}

				try {
					if (tower->shootDecal) {
						tower->shootDecal->update(deltaTime);
						tower->shootDecalElapsed += deltaTime;
						if (tower->shootDecal->currentFrame >= tower->shootDecal->frames.size() || tower->shootDecalElapsed >= decalTime) {
							tower->shootDecal.reset();
							tower->shootDecalElapsed = 0.0f;
						}
					}
				}
				catch (const Exceptions::TowernatorException& e) {
					throw Exceptions::TowernatorException("Failed to update shoot decal: " + std::string(e.what()));
				}
			}
		}
		catch (const Exceptions::TowernatorException& e) {
			std::cerr << "Error processing tower data: " << e.what() << "\n";
		}
		catch (const std::exception& e) {
			std::cerr << "Unexpected error in processTowerData: " << e.what() << "\n";
		}
	}

	void Game::renderTowerData()
	{
		try {
			for (const auto& tower : towers)
			{
				try {
					if (textureCache.find(tower->path) == textureCache.end()) {
						throw Exceptions::TowernatorException("Tower texture not found in cache: " + tower->path);
					}
					renderImage(
						textureCache.at(tower->path),
						GameToWindowCoords(tower->position, WINDOW_HEIGHT),
						tower->rotation
					);
				}
				catch (const std::exception& e) {
					throw Exceptions::TowernatorException("Failed to render tower at position (" +
						std::to_string(tower->position.x) + ", " + std::to_string(tower->position.y) +
						"): " + e.what());
				}
				if (tower->shootDecal) {
					tower->shootDecal->render(
						GameToWindowCoords(tower->position, WINDOW_HEIGHT),
						tower->rotation
					);
				}
			}
		}
		catch (const Exceptions::TowernatorException& e) {
			std::cerr << "Error rendering tower data: " << e.what() << "\n";
		}
		catch (const std::exception& e) {
			std::cerr << "Unexpected error in renderTowerData: " << e.what() << "\n";
		}
	}

	void Tower::scanEnemies(Enemy* target, float deltatime, float rateOfFire, float damage)
	{
		cooldown = std::max(cooldown - deltatime, 0.0f);
		if (cooldown == 0 && target)
		{
			cooldown = rateOfFire;
			target->health -= damage;

			shootDecalElapsed = 0.0f;
			target->hitDecalElapsed = 0.0f;
			if (!shootDecal)
			{
				shootDecal.emplace();
				shootDecal->load("Saules_sprites/Towers/fast_tower_shot");
			}
			target->hitDecal.emplace();
			target->hitDecal->load("Saules_sprites/Enemies/robot_enemy_blow");

			std::cout << "Shot fired, remaining enemy health: " << target->health << "\n";
		}
	}
}
