#pragma once
#include <string>
#include <iostream>
#include <functional>
#include <math.h>
#include "GameObjects.hpp"
#include <SFML/Graphics.hpp>
namespace GameObjects
{
	static sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
	{
		return start + t * (end - start);
	}
	static float dist(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return std::hypot(b.x - a.x, b.y - a.y);
	}

	static sf::Vector2f interpolatePath(const std::vector<sf::Vector2i>& points, float t)
	{
		if (points.size() < 2)
			return sf::Vector2f(points.front());

		// Compute segment lengths
		std::vector<float> lengths;
		float totalLength = 0.0f;
		for (size_t i = 1; i < points.size(); ++i)
		{
			float len = std::hypot(
				static_cast<float>(points[i].x - points[i - 1].x),
				static_cast<float>(points[i].y - points[i - 1].y)
			);
			lengths.push_back(len);
			totalLength += len;
		}

		float distance = t * totalLength;

		// Find which segment the distance falls into
		for (size_t i = 0; i < lengths.size(); ++i)
		{
			if (distance <= lengths[i])
			{
				float localT = distance / lengths[i];
				sf::Vector2f a(points[i]);
				sf::Vector2f b(points[i + 1]);
				return a + (b - a) * localT;
			}
			distance -= lengths[i];
		}

		return sf::Vector2f(points.back());
	}



	static sf::Vector2f interpolatePosition(std::vector<sf::Vector2i>& points, float t)
	{
		t = std::clamp(t, 0.0f, 1.0f);
		return lerp(static_cast<sf::Vector2f>(points.front()), static_cast<sf::Vector2f>(points.back()), t);
	}

	sf::Vector2f Game::GameToWindowCoords(sf::Vector2f coords) const
	{
		return static_cast<sf::Vector2f>(coords - sf::Vector2f(1, 1))* (WINDOW_HEIGHT / 9.0f);
	}

	std::unique_ptr<Enemy> EnemyFactory::createEnemy(const EnemyType& type)
	{
		//TODO: file loading
		if (type == EnemyType::RUNNER) return std::make_unique<Runner>(50, 30, 0.05f, 40, std::string("ff0000"));
		if (type == EnemyType::WALKER) return std::make_unique<Walker>(50, 30, 0.015f, 40, std::string("00ff00"));
		if (type == EnemyType::TANK) return std::make_unique<Tank>(50, 30, 10, 0.01f, std::string("0000ff"));
		if (type == EnemyType::BOSS) return std::make_unique<Boss>(50, 30, 10, 0.0075f, std::string("000000"));
		return nullptr;
	}

	std::unique_ptr<Tower> TowerFactory::createTower(const TowerType& type)
	{
		if (type == TowerType::FAST) return std::make_unique<Fast>(100, 20, 2.0f, 3, "880000");
		if (type == TowerType::SPLASH) return std::make_unique<Splash>(200, 50, 0.75f, 2.5f, "008800");
		if (type == TowerType::STREAM) return std::make_unique<Stream>(150, 2, 0.05f, 4.0f, "000088");
		return nullptr;
	}

	float Game::enemyToughnessMultiplierFunction(int roundNumber)
	{
		return std::max(log(roundNumber) / log(5), 1.0);
	}
	void Game::togglePause(bool gamePaused)
	{

	}

	//No draw calls allowed as image buffer is cleared every frame
	void Game::initialize(/*Difficulty dif*/)
	{
		//hack way fpr now
		textures.push_back(loadTexture("Saules_sprites/Maps/map1_gp_complete.gif"));
		textures.push_back(loadTexture("Saules_sprites/Enemies/robot_enemy1.gif"));

		state = GameState::ROUND_INIT;

		loadWaveDataFromFile();

		//difficulty will be handled later
		{
			/*switch (dif)
			{
			case Difficulty::EASY:
			{
				gold = 1000;
				centralFactoryHealth = 300;
				startRoundDelay = 10;


				break;
			}
			case Difficulty::MEDIUM:
			{
				gold = 500;
				centralFactoryHealth = 200;
				startRoundDelay = 7;


				break;
			};
			case Difficulty::HARD:
			{
				gold = 200;
				centralFactoryHealth = 150;
				startRoundDelay = 4;


				break;
			};
			case Difficulty::INFINTE:
			{
				gold = 300;
				centralFactoryHealth = 300;
				startRoundDelay = 5;


				break;
			};
			}*/
		}
	}

	//info hardcoded for now
	void Game::loadWaveDataFromFile()
	{
		pathPoints.push_back({ 16, 2 }); //start
		pathPoints.push_back({ 2, 2 });
		pathPoints.push_back({ 2, 8 });
		pathPoints.push_back({ 15, 8 });
		pathPoints.push_back({ 15, 4 });
		pathPoints.push_back({ 4, 4 });
		pathPoints.push_back({ 4, 6 });
		pathPoints.push_back({ 12, 6 }); //end

		waves.resize(3);

		// Wave 1
		waves[0].push_back({ 1, EnemyType::RUNNER, 0.66f, 0 });
		waves[0].push_back({ 0, EnemyType::WALKER, 1.0f, 3 });

		// Wave 2
		waves[1].push_back({ 0, EnemyType::TANK, 2.0f, 5 });
		waves[1].push_back({ 0, EnemyType::WALKER, 0.4f, 5 });

		// Wave 3
		waves[2].push_back({ 0, EnemyType::BOSS, 3.0f, 10 });
		waves[2].push_back({ 1, EnemyType::RUNNER, 0.3f, 10000 });

		gold = 1000;
		centralFactoryHealth = 300;
		startRoundDelay = 2;
	}

	void Game::loadRoundWaveData(int waveNum)
	{
		spawnQueue = {};
		float currentTime = elapsedTime + startRoundDelay;
		float prevEnd = currentTime;

		for (int i = 0; i < waves[waveNum].size(); i++)
		{
			float waveStartTime = prevEnd + waves[waveNum][i].startWaveDelay;

			for (int j = 0; j < waves[waveNum][i].enemyCount; j++)
			{
				spawnQueue.push({ waveStartTime + j * waves[waveNum][i].instanceDelay, waves[waveNum][i].type });
				if (j == waves[waveNum][i].enemyCount - 1)
				{
					prevEnd = waveStartTime + j * waves[waveNum][i].instanceDelay;
				}
				std::cout << waveStartTime + j * waves[waveNum][i].instanceDelay << "s t: " << static_cast<int>(waves[waveNum][i].type) << "\n";
			}
		}
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
	void Game::spawnTower(TowerType type)
	{
		auto tower = TowerFactory::createTower(type);
		if (tower)
		{
			towers.push_back(std::move(tower));
		}
	}
	void Game::drawGrid(int rows, int cols) {
		// initialize values
		int numLines = rows + cols - 2;
		sf::VertexArray grid(sf::PrimitiveType::Lines, 2 * (numLines));
		(*window).setView((*window).getDefaultView());
		auto size = (*window).getView().getSize();
		float rowH = size.y / rows;
		float colW = size.x / cols;
		// row separators
		for (int i = 0; i < rows - 1; i++) {
			int r = i + 1;
			float rowY = rowH * r;
			grid[i * 2].position = { 0, rowY };
			grid[i * 2 + 1].position = { size.x, rowY };
		}
		// column separators
		for (int i = rows - 1; i < numLines; i++) {
			int c = i - rows + 2;
			float colX = colW * c;
			grid[i * 2].position = { colX, 0 };
			grid[i * 2 + 1].position = { colX, size.y };
		}
		// draw it
		(*window).draw(grid);
	}
	void Game::update()
	{
		renderBackground();
		processEnemyData();
		processTowerData();

		if (spawnQueue.empty())
		{
			state = GameState::ROUND_INIT;
		}
		if ((roundNumber > 3 && spawnQueue.empty()) || centralFactoryHealth <= 0)
		{
			state = GameState::GAME_OVER;
		}
		if (state == GameState::ROUND_INIT)
		{
			loadRoundWaveData(roundNumber - 1);
			state = GameState::ROUND_ACTION;
			roundNumber++;
		}


		if (state == GameState::ROUND_ACTION)
		{
			while (!(spawnQueue.empty()) && spawnQueue.front().spawnTime <= elapsedTime)
			{
				SpawnEvent& event = spawnQueue.front();
				spawnEnemy(event.type);
				spawnQueue.pop();
			}
		}
	}

	void Game::processEnemyData()
	{
		for (const auto& enemy : enemies)
		{
			enemy->progressInPath += enemy->speed * deltaTime;
			renderImage(textures[1], /*sf::Vector2f(1920 * enemy->progressInPath, 0)*/ GameToWindowCoords(interpolatePath(pathPoints, enemy->progressInPath)));

			if (enemy->progressInPath >= 1.0f)
			{
				centralFactoryHealth -= enemy->damage;
				//delete &enemy;
			}
		}
	}

	void Game::processTowerData()
	{
		for (const auto& tower : towers)
		{

		}
	}
	void Game::renderBackground()
	{
		sf::Vector2u windowSize = (*window).getSize();
		sf::Sprite windowSprite = sf::Sprite(textures[0]);
		sf::Vector2u textureSize = textures[0].getSize();

		float scale = std::min(float(windowSize.x) / textureSize.x, float(windowSize.y) / textureSize.y);
		textureScale = scale;

		windowSprite.setScale(sf::Vector2f(scale, scale));

		(*window).draw(windowSprite);
	}

	sf::Texture& Game::loadTexture(const std::filesystem::path& filename)
	{
		auto it = textureCache.find(filename);
		if (it != textureCache.end())
		{
			return it->second;
		}

		sf::Texture texture;
		if (!texture.loadFromFile(filename))
		{
			throw std::runtime_error("Failed to load texture from file: " + filename.string());
		}

		textureCache[filename] = std::move(texture);
		return textureCache[filename];
	}

	void Game::renderImage(sf::Texture texture, std::optional<sf::Vector2f> pos)
	{
		sf::Sprite sprite = sf::Sprite(texture);
		sprite.setOrigin(sf::Vector2f(0, 0));
		if (!pos.has_value())
		{
			sprite.setPosition({ 300, 300 });
		}
		else
		{
			sprite.setPosition(pos.value());
		}
		sprite.setScale(sf::Vector2f(textureScale, textureScale));
		(*window).draw(sprite);
	}

	void Game::end()
	{
		return;
	}
}