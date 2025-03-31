//Towernator class definition file
//Default naming is camelCase, enums are UPPER_SNAKE_CASE, Class names begin with Uppercase

//color field will be replaced with sf::Sprite and sf::Texture later

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <SFML/Graphics.hpp>

namespace GameObjects
{
	enum class TowerType { FAST, SPLASH, LASER };
	enum class EnemyType { RUNNER, NORMAL, TANK, BOSS };
	enum class Difficulty { HARD, MEDIUM, EASY, INFINITE };
	enum class GameState { MENU, ROUND, GAME_OVER };

	class Enemy
	{
	public:
		int health;
		float speed;
		int reward;
		std::string color;
		float progressInPath;

		virtual ~Enemy() = default;
	};

	class Runner : public Enemy {};
	class Normal : public Enemy {};
	class Tank : public Enemy {};
	class Boss : public Enemy {};

	class EnemyFactory
	{
	public:
		static std::unique_ptr<Enemy> createEnemy(const EnemyType& type);
	};

	class Tower
	{
	public:
		int price;
		int damagePerBullet;
		float rateOfFire;
		float range;
		std::string color;

		virtual ~Tower() = default;
	};

	class TowerFactory
	{
	public:
		static std::unique_ptr<Tower> createTower(const TowerType& type);
	};


	class Fast : public Tower {};
	class Splash : public Tower {};
	class Laser : public Tower {};

	class Tile
	{
	public:
		int x, y;
		std::string texture;

		virtual ~Tile() = default;
	};

	class Background : public Tile {};
	class TowerSpot : public Tile {};
	class EnemyPath : public Tile {};

	struct WaveEnemyData
	{
		int enemyCount;
		GameObjects::Enemy type;
		float instanceDelay;
		int startWaveDelay;
	};

	class Game
	{
	private:
		Game() = default;

	public:
		static Game& getInstance()
		{
			static Game instance;
			return instance;
		}
		Game(const Game&) = delete;
		void operator=(const Game&) = delete;

		float enemyToughnessMultiplierFunction(int roundNumber);
		float levelDifficultyMultiplier = 1.0;

		//Enemy wave info data
		std::vector<std::vector<WaveEnemyData>> enemyRounds;
		int startRoundDelay = 0;

		//Dynamic data
		int roundNumber = 1;
		int gold = 0;
		int centralFactoryHealth = 0;

		std::vector<std::unique_ptr<Tower>> towers;
		std::vector<std::unique_ptr<Enemy>> enemies;

		void spawnEnemy(EnemyType type);
		void spawnTower(TowerType type);

		void togglePause(bool gamePaused);
		void initialize(GameObjects::Difficulty dif);
		void run();
		void end();
	};

}