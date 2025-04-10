#include "GameObjects.hpp"

namespace GameObjects
{
	void Game::renderBackground(sf::Texture texture)
	{
		sf::Vector2u windowSize = (*window).getSize();
		sf::Sprite windowSprite = sf::Sprite(texture);
		sf::Vector2u textureSize = texture.getSize();

		float scale = std::min(float(windowSize.x) / textureSize.x, float(windowSize.y) / textureSize.y);
		textureScale = scale;

		windowSprite.setScale(sf::Vector2f(scale, scale));

		(*window).draw(windowSprite);
	}

	void Game::loadTextureIntoBuffer(const std::filesystem::path& filename)
	{
		sf::Texture tex;
		tex.loadFromFile(filename);
		textureCache[filename.string()] = std::move(tex);
	}

	void Game::renderImage(sf::Texture texture, std::optional<sf::Vector2f> pos, std::optional<float> rotDeg)
	{
		sf::Sprite sprite = sf::Sprite(texture);

		sprite.setScale(sf::Vector2f(textureScale, textureScale));

		if (pos.has_value())
		{
			sprite.setPosition(pos.value());
		}
		if (rotDeg.has_value())
		{
			sprite.setOrigin(sprite.getLocalBounds().getCenter());
			sprite.setRotation(sf::degrees(rotDeg.value()));
		}
		(*window).draw(sprite);
	}

	void Game::drawGrid(int rows, int cols)
	{
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
}