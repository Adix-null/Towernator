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
#include <queue>
#include <iostream>
#include <functional>
#include <math.h>
#include <SFML/Graphics.hpp>

namespace GameObjects
{
	enum class TowerType { FAST, SPLASH, STREAM };
	enum class EnemyType { RUNNER, WALKER, TANK, BOSS };
	enum class Difficulty { HARD, MEDIUM, EASY, INFINITE };
	enum class GameState { MENU, PAUSE, ROUND_INIT, ROUND_ACTION, GAME_OVER };

	constexpr int WINDOW_WIDTH = 2220 / 2;
	constexpr int WINDOW_HEIGHT = 1080 / 2;

	struct WaveEnemyData
	{
		int enemyCount;
		GameObjects::EnemyType type;
		float instanceDelay;
		int startWaveDelay;
	};
	struct SpawnEvent
	{
		float spawnTime;  // Absolute time from gameplay start to spawn enemy
		GameObjects::EnemyType type;
	};

	class Enemy
	{
	public:
		int health;				//enemy starting health
		int damage;				//damage to central factory
		float speed;			//path progress every made every second, 0-1
		int reward;				//reward in coins for a kill
		std::string color;
		float progressInPath;	//how close the enemy is to the central factory (0 - start, 1 - factory)

		//Constructor
		Enemy(int hlt, int dmg, float spd, int rew, const std::string& c)
			: health(hlt), damage(dmg), speed(spd), reward(rew), color(c), progressInPath(0) {
		}
		virtual ~Enemy() = default;
	};

	class Runner : public Enemy { using Enemy::Enemy; };
	class Walker : public Enemy { using Enemy::Enemy; };
	class Tank : public Enemy { using Enemy::Enemy; };
	class Boss : public Enemy { using Enemy::Enemy; };

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

		//Constructor
		Tower(int prc, int dmg, float rof, float rng, const std::string& c)
			: price(prc), damagePerBullet(dmg), rateOfFire(rof), range(rng), color(c) {
		}
		virtual ~Tower() = default;
	};

	class TowerFactory
	{
	public:
		static std::unique_ptr<Tower> createTower(const TowerType& type);
	};

	class Fast : public Tower { using Tower::Tower; };
	class Splash : public Tower { using Tower::Tower; };
	class Stream : public Tower { using Tower::Tower; };

	class Tile
	{
	public:
		int x, y;
		std::string texture;

		Tile(int xPos, int yPos, const std::string& c)
			: x(xPos), y(yPos), texture(c) {
		}

		virtual ~Tile() = default;
	};

	class Background : public Tile { using Tile::Tile; };
	class TowerSpot : public Tile { using Tile::Tile; };
	class EnemyPath : public Tile { using Tile::Tile; };

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

		float levelDifficultyMultiplier = 1.0;

		//Enemy wave info data
		std::vector<std::vector<WaveEnemyData>> waves;
		int startRoundDelay = 0;

		//Dynamic data
		int roundNumber = 1; //starts from 1
		int gold = 0;
		int centralFactoryHealth = 0;
		int score = 0;
		enum GameState state = GameState::MENU;
		std::queue<SpawnEvent> spawnQueue;
		std::vector<sf::Vector2i> pathPoints;

		std::vector<std::unique_ptr<Tower>> towers;
		std::vector<std::unique_ptr<Enemy>> enemies;
		float deltaTime = 0;
		float elapsedTime = 0;
		sf::RenderWindow* window = nullptr;

		std::unordered_map<std::string, sf::Texture> textureCache;
		float textureScale = 1;

		void spawnEnemy(EnemyType type);
		void spawnTower(TowerType type);

		void togglePause(bool gamePaused);
		void initialize(/*GameObjects::Difficulty dif*/);
		void update();
		void end();

		void loadWaveDataFromFile();
		void loadRoundWaveData(int waveNum);
		void processEnemyData();
		void processTowerData();
		void renderBackground(sf::Texture texture);
		void loadTextureIntoBuffer(const std::filesystem::path& filename);
		void renderImage(sf::Texture texture, std::optional<sf::Vector2f> pos, std::optional<float> rot);
		void drawGrid(int rows, int cols);

	};

	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
	float dist(const sf::Vector2f& a, const sf::Vector2f& b);
	std::pair<sf::Vector2f, float> interpolatePath(const std::vector<sf::Vector2i>& points, float t);
	sf::Vector2f interpolatePosition(std::vector<sf::Vector2i>& points, float t);
	sf::Vector2f GameToWindowCoords(sf::Vector2f coords, int gameTilemapHeight);
	float enemyToughnessMultiplierFunction(int roundNumber);
}