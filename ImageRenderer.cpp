#include "GameObjects.hpp"
#include <regex>

namespace GameObjects
{
	void Game::renderBackground(sf::Texture texture)
	{
		if (!window)
			throw Exceptions::TowernatorException("Render window is not initialized");

		sf::Vector2u windowSize = window->getSize();
		sf::Vector2u textureSize = texture.getSize();

		if (textureSize.x == 0 || textureSize.y == 0)
			throw Exceptions::TowernatorException("Texture has invalid size");

		float scale = std::min(float(windowSize.x) / textureSize.x, float(windowSize.y) / textureSize.y);
		textureScale = scale;

		sf::Sprite windowSprite(texture);
		windowSprite.setScale(sf::Vector2f(scale, scale));

		window->draw(windowSprite);
	}

	void Game::loadTextureIntoBuffer(const std::filesystem::path& filename)
	{
		sf::Texture tex;
		if (!tex.loadFromFile(filename.string()))
			throw Exceptions::TowernatorException("Failed to load texture from file: " + filename.string());

		textureCache[filename.string()] = std::move(tex);
	}

	void Game::renderImage(sf::Texture texture, std::optional<sf::Vector2f> pos, std::optional<float> rotDeg)
	{
		if (!window)
			throw Exceptions::TowernatorException("Render window is not initialized");

		sf::Sprite sprite(texture);
		sprite.setScale(sf::Vector2f(textureScale, textureScale));

		if (pos)
			sprite.setPosition(*pos);

		if (rotDeg)
		{
			sprite.setOrigin(sprite.getLocalBounds().getCenter());
			sprite.setRotation(sf::degrees(*rotDeg));
		}

		window->draw(sprite);
	}

	void Game::drawGrid(int rows, int cols)
	{
		if (!window)
			throw Exceptions::TowernatorException("Render window is not initialized");
		if (rows <= 0 || cols <= 0)
			throw Exceptions::TowernatorException("Grid dimensions must be positive");

		int numLines = rows + cols - 2;
		sf::VertexArray grid(sf::PrimitiveType::Lines, 2 * numLines);
		window->setView(window->getDefaultView());

		auto size = window->getView().getSize();
		float rowH = size.y / rows;
		float colW = size.x / cols;

		for (int i = 0; i < rows - 1; ++i) {
			float rowY = rowH * (i + 1);
			grid[i * 2].position = { 0, rowY };
			grid[i * 2 + 1].position = { size.x, rowY };
		}
		for (int i = rows - 1; i < numLines; ++i) {
			float colX = colW * (i - rows + 2);
			grid[i * 2].position = { colX, 0 };
			grid[i * 2 + 1].position = { colX, size.y };
		}

		window->draw(grid);
	}
}
