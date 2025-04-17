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
		frames.clear();
		currentFrame = 0;
		elapsed = 0.0f;

		//use https://ezgif.com/split to split into frames
		std::regex framePattern(R"(frame_\d+_delay-([\d.]+)s\.gif)");

		//Go through every frame gif in the folder
		for (const auto& entry : std::filesystem::directory_iterator(folderPath))
		{
			std::string filename = entry.path().filename().string();
			std::smatch match;
			//extract delay to next frame
			if (std::regex_match(filename, match, framePattern))
			{
				float delay = std::stof(match[1]);
				std::string fullPath = entry.path().string();

				if (sharedTextureCache.find(fullPath) == sharedTextureCache.end())
				{
					sf::Texture tex;
					tex.loadFromFile(fullPath);
					sharedTextureCache[fullPath] = tex;
				}

				frames.push_back({ fullPath, delay });
			}
		}

		// Sort frames to ensure correct order
		std::sort(frames.begin(), frames.end(), [](const Frame& a, const Frame& b)
			{
				return a.path < b.path;
			});
	}

	void GifAnimator::update(float deltaTime)
	{
		if (frames.empty()) return;

		elapsed += deltaTime;
		if (elapsed >= frames[currentFrame].delay)
		{
			elapsed = 0.0f;
			currentFrame = (currentFrame + 1) % frames.size();
		}
	}

	void GifAnimator::render(const sf::Vector2f& position, std::optional<float> rotDeg)
	{
		if (frames.empty()) return;

		Game& game = Game::getInstance();
		game.renderImage(sharedTextureCache[frames[currentFrame].path], position, rotDeg);

	}
}