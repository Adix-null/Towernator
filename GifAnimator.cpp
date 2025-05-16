#pragma once
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <unordered_map>
#include <regex>
#include "GameObjects.hpp"

namespace GameObjects
{
	void GifAnimator::load(const std::string& folderPath)
	{
		try {
			frames.clear();
			currentFrame = 0;
			elapsed = 0.0f;

			std::regex framePattern(R"(frame_\d+_delay-([\d.]+)s\.gif)");

			for (const auto& entry : std::filesystem::directory_iterator(folderPath))
			{
				std::string filename = entry.path().filename().string();
				std::smatch match;

				if (std::regex_match(filename, match, framePattern))
				{
					float delay = std::stof(match[1]);
					std::string fullPath = entry.path().string();

					if (sharedTextureCache.find(fullPath) == sharedTextureCache.end())
					{
						sf::Texture tex;
						if (!tex.loadFromFile(fullPath))
							throw Exceptions::TowernatorException("Failed to load texture: " + fullPath);

						sharedTextureCache[fullPath] = tex;
					}

					frames.push_back({ fullPath, delay });
				}
			}

			std::sort(frames.begin(), frames.end(), [](const Frame& a, const Frame& b) {
				return a.path < b.path;
				});
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("GifAnimator::load failed: " + std::string(e.what()));
		}
	}

	void GifAnimator::update(float deltaTime)
	{
		try {
			if (frames.empty()) return;

			elapsed += deltaTime * speed;
			if (elapsed >= frames[currentFrame].delay)
			{
				elapsed = 0.0f;
				currentFrame = (currentFrame + 1) % frames.size();
			}
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("GifAnimator::update failed: " + std::string(e.what()));
		}
	}

	void GifAnimator::render(const sf::Vector2f& position, std::optional<float> rotDeg)
	{
		try {
			if (frames.empty()) return;

			Game& game = Game::getInstance();
			game.renderImage(sharedTextureCache.at(frames[currentFrame].path), position, rotDeg);
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("GifAnimator::render failed: " + std::string(e.what()));
		}
	}
}
