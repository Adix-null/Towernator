#include "GameObjects.hpp"

namespace GameObjects
{
	std::unique_ptr<Tower> TowerFactory::createTower(const TowerType& type)
	{
		if (type == TowerType::FAST) return std::make_unique<Fast>(100, 20, 2.0f, 3, "880000");
		if (type == TowerType::SPLASH) return std::make_unique<Splash>(200, 50, 0.75f, 2.5f, "008800");
		if (type == TowerType::STREAM) return std::make_unique<Stream>(150, 2, 0.05f, 4.0f, "000088");
		return nullptr;
	}

	void Game::spawnTower(TowerType type)
	{
		auto tower = TowerFactory::createTower(type);
		if (tower)
		{
			towers.push_back(std::move(tower));
		}
	}
}