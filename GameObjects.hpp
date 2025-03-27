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

	struct RoundEnemyData
	{
		int enemyCount;
		GameObjects::Enemy type;
		float instanceDelay;
		int startDelay;
	};

	enum class Difficulty { HARD, MEDIUM, EASY, INFINITE };
	enum class GameState { MENU, GAME, GAME_OVER };

	extern float EnemyToughnessMultiplierFunction(int roundNumber);
	constexpr float levelDifficultyMultiplier = 1.5f;
	constexpr int startingGold = 500;
	constexpr int factoryHealth = 200;

	extern std::vector<RoundEnemyData> enemyRounds;
	constexpr int PauseBetweenRounds = 15;


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

		int roundNumber = 0;
		int gold = GameObjects::startingGold;
		int centralFactoryHealth = GameObjects::factoryHealth;

		std::vector<std::unique_ptr<Tower>> towers;
		std::vector<std::unique_ptr<Enemy>> enemies;

		Game(const Game&) = delete;
		void operator=(const Game&) = delete;

		void togglePause(bool gamePaused);
		void initialize(GameObjects::Difficulty dif);
		void run();
		void end();
	};

}