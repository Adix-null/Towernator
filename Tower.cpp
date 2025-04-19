#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Tower> TowerFactory::createTower(const TowerType& type, const sf::Vector2f& position)
	{
		if (type == TowerType::FAST) return std::make_unique<Fast>(100, 20, 2.0f, 3.0f, "880000", position);
		if (type == TowerType::SPLASH) return std::make_unique<Splash>(200, 50, 0.75f, 2.5f, "008800", position);
		if (type == TowerType::STREAM) return std::make_unique<Stream>(150, 2, 0.05f, 4.0f, "000088", position);
		return nullptr;
	}

	void Game::spawnTower(TowerType type, const sf::Vector2f& pos)
	{
		auto tower = TowerFactory::createTower(type, pos);
		if (tower)
		{
			towers.push_back(std::move(tower));
		}
	}
}