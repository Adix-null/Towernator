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
#include <cassert>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "Exceptions.cpp"

namespace GameObjects
{
	enum class TowerType { FAST, SPLASH, STREAM };
	enum class EnemyType { RUNNER, WALKER, TANK, BOSS };
	enum class Difficulty { HARD, MEDIUM, EASY, INFINITE };
	enum class GameState { MENU, PAUSE, ROUND_INIT, ROUND_ACTION, GAME_OVER };

	constexpr int WINDOW_WIDTH = 2220 / 1;
	constexpr int WINDOW_HEIGHT = 1080 / 1;

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
	struct Frame
	{
		std::string path;
		float delay;
	};

	class GifAnimator
	{
	public:
		void load(const std::string& folderPath);
		void update(float deltaTime);
		void render(const sf::Vector2f& position, std::optional<float> rotDeg);

		std::vector<Frame> frames;
		std::unordered_map<std::string, sf::Texture> sharedTextureCache;
		std::size_t currentFrame = 0;
		float elapsed = 0.0f;
		float speed = 1.0f;
	};

	class Enemy
	{
	public:
		int health;				//enemy starting health
		int damage;				//damage to central factory
		float speed;			//path progress every made every second, 0-1
		int reward;				//reward in coins for a kill
		const std::string& path;
		float progressInPath;	//how close the enemy is to the central factory (0 - start, 1 - factory)

		std::optional<GifAnimator> hitDecal;
		float hitDecalElapsed = 0.0f;

		//Constructor
		Enemy(int hlt, int dmg, float spd, int rew, const std::string& path)
			: health(hlt), damage(dmg), speed(spd), reward(rew), path(path), progressInPath(0) {
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
		std::string path;
		sf::Vector2f position;
		float rotation = 0.0f;
		float cooldown = 0.0f;

		std::optional<GifAnimator> shootDecal;
		float shootDecalElapsed = 0.0f;

		//Constructor
		Tower(int prc, int dmg, float rof, float rng, const std::string& c, const sf::Vector2f& pos)
			: price(prc), damagePerBullet(dmg), rateOfFire(rof), range(rng), path(c), rotation(0.0f), position(pos) {
		}
		virtual ~Tower() = default;

		void scanEnemies(Enemy* target, float deltatime, float rateOfFire, float damage);
	};

	class TowerFactory
	{
	public:
		static std::unique_ptr<Tower> createTower(const TowerType& type, const sf::Vector2f& position);
	};

	class Fast : public Tower { using Tower::Tower; };
	class Splash : public Tower { using Tower::Tower; };
	class Stream : public Tower { using Tower::Tower; };

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
		float decalTime = 0.33f;

		void spawnEnemy(EnemyType type);
		void spawnTower(TowerType type, const sf::Vector2f& pos);

		void togglePause(bool gamePaused);
		void initialize(/*GameObjects::Difficulty dif*/);
		void update();
		void end();

		void loadWaveDataFromFile(const std::string& filepath, Difficulty dif = Difficulty::EASY);
		void loadRoundWaveData(int waveNum);
		void placeTestTower();
		void processEnemyData();
		void processTowerData();

		void renderEnemyData();
		void renderTowerData();
		void renderBackground(sf::Texture texture);
		void loadTextureIntoBuffer(const std::filesystem::path& filename);
		void renderImage(sf::Texture texture, std::optional<sf::Vector2f> pos, std::optional<float> rot);
		void drawGrid(int rows, int cols);

		std::vector<GifAnimator> animators;
	};

	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);
	float dist(const sf::Vector2f& a, const sf::Vector2f& b);
	std::pair<sf::Vector2f, float> interpolatePath(const std::vector<sf::Vector2i>& points, float t);
	sf::Vector2f interpolatePosition(std::vector<sf::Vector2i>& points, float t);
	sf::Vector2f GameToWindowCoords(sf::Vector2f coords, int gameTilemapHeight);
	float enemyToughnessMultiplierFunction(int roundNumber);
}