#include "gtest/gtest.h"
#include "GameObjects.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <stdexcept>

using namespace GameObjects;

// Success-case tests

TEST(EnemyFactoryTest, CreatesEnemySuccessfully) {
    auto enemy = EnemyFactory::createEnemy(EnemyType::RUNNER);
    ASSERT_NE(enemy, nullptr);
    ASSERT_FLOAT_EQ(enemy->speed, 0.15f);
}

TEST(TowerFactoryTest, CreatesTowerSuccessfully) {
    sf::Vector2f pos(100.0f, 100.0f);
    auto tower = TowerFactory::createTower(TowerType::FAST, pos);
    ASSERT_NE(tower, nullptr);
    ASSERT_EQ(tower->position, pos);
}

TEST(InterpolationTest, InterpolatesPathCorrectly) {
    std::vector<sf::Vector2i> path = { {0, 0}, {10, 0} };
    auto result = interpolatePath(path, 0.5f);
    ASSERT_NEAR(result.first.x, 5.0f, 0.01);
    ASSERT_NEAR(result.first.y, 0.0f, 0.01);
}

TEST(GifAnimatorTest, LoadValidFrames) {
    GifAnimator animator;
    ASSERT_NO_THROW(animator.load("test_data/valid_gif_frames"));
}

// Exception-case tests

TEST(EnemyFactoryTest, ThrowsOnUnknownType) {
    ASSERT_THROW(EnemyFactory::createEnemy(static_cast<EnemyType>(999)), Exceptions::TowernatorException);
}

TEST(GameRenderTowerDataTest, ThrowsWhenTextureMissing) {
    Game& game = Game::getInstance();
    game.textureCache.clear();
    ASSERT_THROW(game.renderTowerData(), Exceptions::TowernatorException);
}

TEST(GifAnimatorTest, ThrowsOnMissingFrame) {
    GifAnimator animator;
    ASSERT_THROW(animator.load("test_data/missing_frames"), Exceptions::TowernatorException);
}

TEST(InterpolatePathTest, ThrowsOnEmptyPoints) {
    std::vector<sf::Vector2i> emptyPoints;
    ASSERT_THROW(interpolatePath(emptyPoints, 0.5f), Exceptions::TowernatorException);
}

TEST(GameRenderImageTest, ThrowsWhenRenderingWithoutWindow) {
    Game& game = Game::getInstance();
    game.window = nullptr;
    sf::Texture dummy;
    ASSERT_THROW(game.renderImage(dummy), Exceptions::TowernatorException);
}

TEST(GameDrawGridTest, ThrowsWhenWindowMissing) {
    Game& game = Game::getInstance();
    game.window = nullptr;
    ASSERT_THROW(game.drawGrid(3, 3), Exceptions::TowernatorException);
}
