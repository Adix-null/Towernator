#include "GameObjects.hpp"

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
    assert(distance == 5.0f);
    std::cout << "testDistFunction passed\n";
}

void testInterpolatePosition() {
    std::vector<sf::Vector2i> path = { {0, 0}, {10, 10}, {20, 0} };
    auto position = interpolatePosition(path, 0.5f);
    assert(position.x == 10.0f && position.y == 10.0f);
    std::cout << "testInterpolatePosition passed\n";
}

void testGifAnimatorLoadValid() {
    GifAnimator animator;
    try {
        animator.load("test_data/valid_gif_frames");
        std::cout << "testGifAnimatorLoadValid passed\n";
    }
    catch (const std::exception& e) {
        std::cerr << "testGifAnimatorLoadValid failed: " << e.what() << "\n";
        assert(false);
    }
}

void testTextureLoading() {
    Game& game = Game::getInstance();
    try {
        game.loadTextureIntoBuffer("test_data/valid_texture.png");
        assert(game.textureCache.find("test_data/valid_texture.png") != game.textureCache.end());
        std::cout << "testTextureLoading passed\n";
    }
    catch (const std::exception& e) {
        std::cerr << "testTextureLoading failed: " << e.what() << "\n";
        assert(false);
    }
}





void testLoadWaveDataFromValidFile() {
    Game& game = Game::getInstance();
    try {
        game.loadWaveDataFromFile("wave_data.txt", Difficulty::EASY);

        assert(!game.pathPoints.empty());
        assert(game.pathPoints.size() == 8);
        assert(game.pathPoints[0].x == 16 && game.pathPoints[0].y == 2);

        assert(game.gold == 1000);
        std::cout << game.centralFactoryHealth;
        assert(game.centralFactoryHealth == 300);
        assert(game.startRoundDelay == 10);

        assert(game.waves.size() == 3);
        assert(!game.waves[0].empty());

        std::cout << "testLoadWaveDataFromValidFile passed\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::cerr << "testLoadWaveDataFromValidFile failed: " << e.what() << "\n";
        assert(false);
    }
}

void testLoadWaveDataDifferentDifficulties() {
    Game& game = Game::getInstance();

    try {
        game.loadWaveDataFromFile("wave_data.txt", Difficulty::HARD);
        assert(game.gold == 200);
        assert(game.centralFactoryHealth == 150);
        assert(game.startRoundDelay == 4);
        std::cout << "testLoadWaveDataDifferentDifficulties (HARD) passed\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::cerr << "testLoadWaveDataDifferentDifficulties (HARD) failed: " << e.what() << "\n";
        assert(false);
    }

    try {
        game.loadWaveDataFromFile("wave_data.txt", Difficulty::MEDIUM);
        assert(game.gold == 500);
        assert(game.centralFactoryHealth == 200);
        assert(game.startRoundDelay == 7);
        std::cout << "testLoadWaveDataDifferentDifficulties (MEDIUM) passed\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::cerr << "testLoadWaveDataDifferentDifficulties (MEDIUM) failed: " << e.what() << "\n";
        assert(false);
    }
}

void testLoadWaveDataFileNotFound() {
    Game& game = Game::getInstance();

    try {
        game.loadWaveDataFromFile("nonexistent_wave_data.txt", Difficulty::EASY);
        std::cerr << "testLoadWaveDataFileNotFound failed: expected exception but none was thrown\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::string errorMsg = e.what();
        if (errorMsg.find("Could not open nonexistent_wave_data.txt") != std::string::npos) {
            std::cout << "testLoadWaveDataFileNotFound passed\n";
        }
        else {
            std::cerr << "testLoadWaveDataFileNotFound failed: wrong error message: " << errorMsg << '\n';
        }
    }
    catch (...) {
        std::cerr << "testLoadWaveDataFileNotFound failed: unexpected exception type\n";
    }
}

void testLoadWaveDataInvalidEnemyType() {
    Game& game = Game::getInstance();

    std::ofstream tempFile("test_wave_data_invalid.txt");
    tempFile << "[GAME_SETTINGS]\n";
    tempFile << "waveCount 1\n";
    tempFile << "[DIFFICULTY_EASY]\n";
    tempFile << "gold 1000\n";
    tempFile << "centralFactoryHealth 300\n";
    tempFile << "startRoundDelay 10\n";
    tempFile << "[PATH]\n";
    tempFile << "0 0\n";
    tempFile << "10 10\n";
    tempFile << "[WAVE1]\n";
    tempFile << "5 INVALID_ENEMY 1.0 0\n";
    tempFile.close();


    try {
        game.loadWaveDataFromFile("test_wave_data_invalid.txt", Difficulty::EASY);
        std::cerr << "testLoadWaveDataInvalidEnemyType failed: expected exception but none was thrown\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::string errorMsg = e.what();
        if (errorMsg.find("Unknown enemy type: INVALID_ENEMY") != std::string::npos) {
            std::cout << "testLoadWaveDataInvalidEnemyType passed\n";
        }
        else {
            std::cerr << "testLoadWaveDataInvalidEnemyType failed: wrong error message: " << errorMsg << '\n';
        }
    }
    catch (...) {
        std::cerr;
    }
}

void testLoadWaveDataInvalidWaveFormat() {
    Game& game = Game::getInstance();

    std::ofstream tempFile("test_wave_data_format.txt");
    tempFile << "[GAME_SETTINGS]\n";
    tempFile << "waveCount 1\n";
    tempFile << "[DIFFICULTY_EASY]\n";
    tempFile << "gold 1000\n";
    tempFile << "centralFactoryHealth 300\n";
    tempFile << "startRoundDelay 10\n";
    tempFile << "[PATH]\n";
    tempFile << "0 0\n";
    tempFile << "10 10\n";
    tempFile << "[WAVE1]\n";
    tempFile << "5 RUNNER\n";
    tempFile.close();

    try {
        game.loadWaveDataFromFile("test_wave_data_format.txt", Difficulty::EASY);
        assert(false);
    }
    catch (const Exceptions::TowernatorException& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("Invalid wave data format") != std::string::npos);
        std::cout << "testLoadWaveDataInvalidWaveFormat passed\n";
    }

    std::remove("test_wave_data_format.txt");
}

void testLoadWaveDataNegativeValues() {
    Game& game = Game::getInstance();

    std::ofstream tempFile("test_wave_data_negative.txt");
    tempFile << "[GAME_SETTINGS]\n";
    tempFile << "waveCount 1\n";
    tempFile << "[DIFFICULTY_EASY]\n";
    tempFile << "gold 1000\n";
    tempFile << "centralFactoryHealth 300\n";
    tempFile << "startRoundDelay 10\n";
    tempFile << "[PATH]\n";
    tempFile << "0 0\n";
    tempFile << "10 10\n";
    tempFile << "[WAVE1]\n";
    tempFile << "-5 RUNNER 1.0 0\n";
    tempFile.close();

    try {
        game.loadWaveDataFromFile("test_wave_data_negative.txt", Difficulty::EASY);
        assert(false);
    }
    catch (const Exceptions::TowernatorException& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("Enemy count must be positive") != std::string::npos);
        std::cout << "testLoadWaveDataNegativeValues passed\n";
    }

    std::remove("test_wave_data_negative.txt");
}

void testLoadWaveDataOutOfBoundsWave() {
    Game& game = Game::getInstance();

    std::ofstream tempFile("test_wave_data_bounds.txt");
    tempFile << "[GAME_SETTINGS]\n";
    tempFile << "waveCount 1\n";
    tempFile << "[DIFFICULTY_EASY]\n";
    tempFile << "gold 1000\n";
    tempFile << "centralFactoryHealth 300\n";
    tempFile << "startRoundDelay 10\n";
    tempFile << "[PATH]\n";
    tempFile << "0 0\n";
    tempFile << "10 10\n";
    tempFile << "[WAVE2]\n";
    tempFile << "5 RUNNER 1.0 0\n";
    tempFile.close();

    try {
        game.loadWaveDataFromFile("test_wave_data_bounds.txt", Difficulty::EASY);
        assert(false);
    }
    catch (const Exceptions::TowernatorException& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("Wave index 2 is out of bounds") != std::string::npos);
        std::cout << "testLoadWaveDataOutOfBoundsWave passed\n";
    }

    std::remove("test_wave_data_bounds.txt");
}

void testLoadWaveDataEmptyFile() {
    Game& game = Game::getInstance();

    std::ofstream tempFile("test_wave_data_empty.txt");
    tempFile.close();

    try {
        game.loadWaveDataFromFile("test_wave_data_empty.txt", Difficulty::EASY);
        assert(false);
    }
    catch (const Exceptions::TowernatorException& e) {
        std::string errorMsg = e.what();
        assert(errorMsg.find("No path points found") != std::string::npos);
        std::cout << "testLoadWaveDataEmptyFile passed\n";
    }

    std::remove("test_wave_data_empty.txt");
}

void testLoadWaveDataValidation() {
    Game& game = Game::getInstance();

    try {
        game.loadWaveDataFromFile("wave_data.txt", Difficulty::EASY);

        assert(!game.pathPoints.empty());
        assert(game.waves.size() > 0);

        bool hasWaveData = false;
        for (const auto& wave : game.waves) {
            if (!wave.empty()) {
                hasWaveData = true;
                break;
            }
        }
        assert(hasWaveData);

        std::cout << "testLoadWaveDataValidation passed\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::cerr << "testLoadWaveDataValidation failed: " << e.what() << "\n";
        assert(false);
    }
}

void testLoadWaveDataDefaultDifficulty() {
    Game& game = Game::getInstance();
    try {
        // Test that default parameter works (should default to EASY)
        game.loadWaveDataFromFile("wave_data.txt");

        // Verify EASY difficulty settings are loaded
        assert(game.gold == 1000);
        assert(game.centralFactoryHealth == 300);
        assert(game.startRoundDelay == 10);

        std::cout << "testLoadWaveDataDefaultDifficulty passed\n";
    }
    catch (const Exceptions::TowernatorException& e) {
        std::cerr << "testLoadWaveDataDefaultDifficulty failed: " << e.what() << "\n";
        assert(false);
    }
}

void setupTestEnvironment() {
    std::cout << "Setting up test environment...\n";

    try {
        std::filesystem::create_directories("test_data");
        std::cout << "Created test_data directory\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating test_data directory: " << e.what() << "\n";
    }

    std::ofstream testTexture("test_data/valid_texture.png", std::ios::binary);
    if (testTexture.is_open()) {
        unsigned char pngData[] = {
            0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
            0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
            0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0x15, 0xC4, 0x89, 0x00, 0x00, 0x00,
            0x0A, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0x63, 0x00, 0x01, 0x00, 0x00,
            0x05, 0x00, 0x01, 0x0D, 0x0A, 0x2D, 0xB4, 0x00, 0x00, 0x00, 0x00, 0x49,
            0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
        };
        testTexture.write(reinterpret_cast<char*>(pngData), sizeof(pngData));
        testTexture.close();
        std::cout << "Created valid_texture.png\n";
    }
    else {
        std::cerr << "Error creating test texture file\n";
    }

    try {
        std::filesystem::create_directories("test_data/valid_gif_frames");
        std::cout << "Created valid_gif_frames directory\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating valid_gif_frames directory: " << e.what() << "\n";
    }

    if (!std::filesystem::exists("wave_data.txt")) {
        std::ofstream waveFile("wave_data.txt");
        if (waveFile.is_open()) {
            waveFile << "[GAME_SETTINGS]\n";
            waveFile << "waveCount 3\n\n";
            waveFile << "[DIFFICULTY_EASY]\n";
            waveFile << "gold 1000\n";
            waveFile << "centralFactoryHealth 300\n";
            waveFile << "startRoundDelay 10\n\n";
            waveFile << "[DIFFICULTY_MEDIUM]\n";
            waveFile << "gold 500\n";
            waveFile << "centralFactoryHealth 200\n";
            waveFile << "startRoundDelay 7\n\n";
            waveFile << "[DIFFICULTY_HARD]\n";
            waveFile << "gold 200\n";
            waveFile << "centralFactoryHealth 150\n";
            waveFile << "startRoundDelay 4\n\n";
            waveFile << "[DIFFICULTY_INFINITE]\n";
            waveFile << "gold 300\n";
            waveFile << "centralFactoryHealth 300\n";
            waveFile << "startRoundDelay 5\n\n";
            waveFile << "[PATH]\n";
            waveFile << "16 2\n2 2\n2 8\n15 8\n15 4\n4 4\n4 6\n12 6\n\n";
            waveFile << "[WAVE1]\n";
            waveFile << "10 RUNNER 0.66 0\n";
            waveFile << "5 WALKER 1.0 3\n\n";
            waveFile << "[WAVE2]\n";
            waveFile << "7 TANK 2.0 5\n";
            waveFile << "5 WALKER 0.4 5\n\n";
            waveFile << "[WAVE3]\n";
            waveFile << "1 BOSS 3.0 10\n";
            waveFile << "15 RUNNER 0.3 10000\n";
            waveFile.close();
            std::cout << "Created wave_data.txt file\n";
        }
        else {
            std::cerr << "Error creating wave_data.txt file\n";
        }
    }
    else {
        std::cout << "wave_data.txt already exists\n";
    }

    std::cout << "Test environment setup complete\n";
}

void cleanupTestEnvironment() {
    std::cout << "Cleaning up test environment...\n";

    std::remove("test_data/valid_texture.png");

    try {
        if (std::filesystem::exists("test_data")) {
            std::filesystem::remove_all("test_data");
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error during cleanup: " << e.what() << '\n';
    }

    std::cout << "Cleanup complete\n";
}

void runBasicTests() {
    std::cout << "\nRunning Basic Game Object Tests\n";

    testEnemyFactoryCreatesEnemy();
    testTowerFactoryCreatesTower();
    testLerpFunction();
    testDistFunction();
    testInterpolatePosition();
    testInterpolatePath();

    std::cout << "All basic tests passed!\n";
}

void runGraphicsTests() {
    std::cout << "\nRunning Graphics and Rendering Tests\n";

    testGifAnimatorLoadValid();
    testTextureLoading();

    std::cout << "All graphics tests passed!\n";
}


void runWaveDataTests() {
    std::cout << "\nRunning Wave Data Loading Tests\n";

    testLoadWaveDataFromValidFile();
    testLoadWaveDataDifferentDifficulties();
    testLoadWaveDataFileNotFound();
    testLoadWaveDataInvalidEnemyType();
    testLoadWaveDataInvalidWaveFormat();
    testLoadWaveDataNegativeValues();
    testLoadWaveDataOutOfBoundsWave();
    testLoadWaveDataEmptyFile();
    testLoadWaveDataValidation();
    testLoadWaveDataDefaultDifficulty();

    std::cout << "All wave data tests passed!\n";
}

void printTestSummary(int totalTests, int passedTests) {
    std::cout << "\nTEST SUMMARY\n";
    std::cout << "Total Tests: " << totalTests << "\n";
    std::cout << "Passed: " << passedTests << "\n";
    std::cout << "Failed: " << (totalTests - passedTests) << "\n";

    if (passedTests == totalTests) {
        std::cout << "ALL TESTS PASSED\n";
    }
    else {
        std::cout << "SOME TESTS FAILED\n";
    }
}

int main() {
    std::cout << "TOWERNATOR GAME TEST SUITE\n";

    int totalTestSuites = 3;
    int passedTestSuites = 0;

    try {
        setupTestEnvironment();

        try {
            runBasicTests();
            passedTestSuites++;
        }
        catch (const std::exception& e) {
            std::cerr << "Basic tests failed: " << e.what() << "\n";
        }

        try {
            runGraphicsTests();
            passedTestSuites++;
        }
        catch (const std::exception& e) {
            std::cerr << "Graphics tests failed: " << e.what() << "\n";
        }

        try {
            runWaveDataTests();
            passedTestSuites++;
        }
        catch (const std::exception& e) {
            std::cerr << "Wave data tests failed: " << e.what() << "\n";
        }

        printTestSummary(totalTestSuites, passedTestSuites);

        cleanupTestEnvironment();

        return (passedTestSuites == totalTestSuites) ? 0 : 1;

    }
    catch (const std::exception& e) {
        std::cerr << "Fatal error during testing: " << e.what() << "\n";
        cleanupTestEnvironment();
        return 1;
    }
    catch (...) {
        std::cerr << "Unknown fatal error during testing\n";
        cleanupTestEnvironment();
        return 1;
    }
}