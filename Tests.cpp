#include "GameObjects.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>

using namespace GameObjects;

void testEnemyFactoryCreatesEnemy() {
    auto enemy = EnemyFactory::createEnemy(EnemyType::RUNNER);
    assert(enemy != nullptr);
    assert(enemy->speed == 0.15f);
    std::cout << "testEnemyFactoryCreatesEnemy passed\n";
}

void testTowerFactoryCreatesTower() {
    sf::Vector2f pos(100.0f, 100.0f);
    auto tower = TowerFactory::createTower(TowerType::FAST, pos);
    assert(tower != nullptr);
    assert(tower->position == pos);
    std::cout << "testTowerFactoryCreatesTower passed\n";
}

void testInterpolatePath() {
    std::vector<sf::Vector2i> path = { {0, 0}, {10, 0} };
    auto result = interpolatePath(path, 0.5f);
    assert(std::abs(result.first.x - 5.0f) < 0.01f);
    assert(std::abs(result.first.y) < 0.01f);
    std::cout << "testInterpolatePath passed\n";
}

void testGifAnimatorLoadValid() {
    GifAnimator animator;
    try {
        animator.load("test_data/valid_gif_frames");
        std::cout << "testGifAnimatorLoadValid passed
void testLerpFunction() {
    sf::Vector2f start(0, 0);
    sf::Vector2f end(10, 10);
    auto result = lerp(start, end, 0.5f);
    assert(result.x == 5.0f && result.y == 5.0f);
    std::cout << "testLerpFunction passed\n";
}

void testDistFunction() {
    sf::Vector2f point1(0, 0);
    sf::Vector2f point2(3, 4);
    float distance = dist(point1, point2);
    assert(distance == 5.0f); // 3-4-5 triangle
    std::cout << "testDistFunction passed\n";
}

void testInterpolatePosition() {
    std::vector<sf::Vector2i> path = {{0, 0}, {10, 10}, {20, 0}};
    auto position = interpolatePosition(path, 0.5f); // Should interpolate between (0, 0) and (10, 10)
    assert(position.x == 10.0f && position.y == 10.0f);
    std::cout << "testInterpolatePosition passed\n";
}

void testTextureLoading() {
    Game& game = Game::getInstance();
    try {
        game.loadTextureIntoBuffer("test_data/valid_texture.png");
        assert(game.textureCache.find("test_data/valid_texture.png") != game.textureCache.end());
        std::cout << "testTextureLoading passed\n";
    } catch (...) {
        std::cerr << "testTextureLoading failed\n";
        assert(false);
    }
}

void testTextureNotFoundInCache() {
    Game& game = Game::getInstance();
    try {
        game.renderImage(sf::Texture(), {0.0f, 0.0f});
        assert(false); // Should not reach here, exception should be thrown
    } catch (const Exceptions::TowernatorException&) {
        std::cout << "testTextureNotFoundInCache passed\n";
    }
}

void testWindowNotInitialized() {
    Game& game = Game::getInstance();
    game.window = nullptr;
    try {
        game.renderBackground(sf::Texture());
        assert(false); // Should not reach here
    } catch (const Exceptions::TowernatorException&) {
        std::cout << "testWindowNotInitialized passed\n";
    }
}

void testGameDrawGridWithInvalidRowsCols() {
    Game& game = Game::getInstance();
    game.window = nullptr;
    try {
        game.drawGrid(0, 0); // Invalid grid with 0 rows and 0 cols
        assert(false); // Should not reach here
    } catch (const Exceptions::TowernatorException&) {
        std::cout << "testGameDrawGridWithInvalidRowsCols passed\n";
    }
}

void testInvalidTowerCreation() {
    sf::Vector2f pos(100.0f, 100.0f);
    try {
        auto tower = TowerFactory::createTower(static_cast<TowerType>(999), pos); // Invalid tower type
        assert(false); // Should not reach here
    } catch (const Exceptions::TowernatorException&) {
        std::cout << "testInvalidTowerCreation passed\n";
    }
}
void testLerpFunction() {
    sf::Vector2f start(0, 0);
    sf::Vector2f end(10, 10);
    auto result = lerp(start, end, 0.5f);
    assert(result.x == 5.0f && result.y == 5.0f);
    std::cout << "testLerpFunction passed\n";
}

void testDistFunction() {
    sf::Vector2f point1(0, 0);
    sf::Vector2f point2(3, 4);
    float distance = dist(point1, point2);
    assert(distance == 5.0f); // 3-4-5 triangle
    std::cout << "testDistFunction passed\n";
}

void testInterpolatePosition() {
    std::vector<sf::Vector2i> path = { {0, 0}, {10, 10}, {20, 0} };
    auto position = interpolatePosition(path, 0.5f); // Should interpolate between (0, 0) and (10, 10)
    assert(position.x == 10.0f && position.y == 10.0f);
    std::cout << "testInterpolatePosition passed\n";
}

void testTextureLoading() {
    Game& game = Game::getInstance();
    try {
        game.loadTextureIntoBuffer("test_data/valid_texture.png");
        assert(game.textureCache.find("test_data/valid_texture.png") != game.textureCache.end());
        std::cout << "testTextureLoading passed\n";
    }
    catch (...) {
        std::cerr << "testTextureLoading failed\n";
        assert(false);
    }
}

void testTextureNotFoundInCache() {
    Game& game = Game::getInstance();
    try {
        game.renderImage(sf::Texture(), { 0.0f, 0.0f });
        assert(false); // Should not reach here, exception should be thrown
    }
    catch (const Exceptions::TowernatorException&) {
        std::cout << "testTextureNotFoundInCache passed\n";
    }
}

void testWindowNotInitialized() {
    Game& game = Game::getInstance();
    game.window = nullptr;
    try {
        game.renderBackground(sf::Texture());
        assert(false); // Should not reach here
    }
    catch (const Exceptions::TowernatorException&) {
        std::cout << "testWindowNotInitialized passed\n";
    }
}

void testGameDrawGridWithInvalidRowsCols() {
    Game& game = Game::getInstance();
    game.window = nullptr;
    try {
        game.drawGrid(0, 0); // Invalid grid with 0 rows and 0 cols
        assert(false); // Should not reach here
    }
    catch (const Exceptions::TowernatorException&) {
        std::cout << "testGameDrawGridWithInvalidRowsCols passed\n";
    }
}

void testInvalidTowerCreation() {
    sf::Vector2f pos(100.0f, 100.0f);
    try {
        auto tower = TowerFactory::createTower(static_cast<TowerType>(999), pos); // Invalid tower type
        assert(false); // Should not reach here
    }
    catch (const Exceptions::TowernatorException&) {
        std::cout << "testInvalidTowerCreation passed\n";
    }
}
