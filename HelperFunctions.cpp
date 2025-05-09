#include "GameObjects.hpp"

namespace GameObjects
{
	sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
	{
		return start + t * (end - start);
	}

	float dist(const sf::Vector2f& a, const sf::Vector2f& b)
	{
		return std::hypot(b.x - a.x, b.y - a.y);
	}

	std::pair<sf::Vector2f, float> interpolatePath(const std::vector<sf::Vector2i>& points, float t)
	{
		try {
			if (points.size() < 2)
				throw Exceptions::TowernatorException("interpolatePath: path must contain at least two points");

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

			for (size_t i = 0; i < lengths.size(); ++i)
			{
				if (distance <= lengths[i])
				{
					float localT = distance / lengths[i];
					sf::Vector2f a(points[i]);
					sf::Vector2f b(points[i + 1]);

					sf::Vector2f dir = b - a;
					float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;

					sf::Vector2f pos = a + (b - a) * localT;
					return { pos, angle };
				}
				distance -= lengths[i];
			}

			return { sf::Vector2f(points.back()), 0.0f };
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("interpolatePath failed: " + std::string(e.what()));
		}
	}

	sf::Vector2f interpolatePosition(std::vector<sf::Vector2i>& points, float t)
	{
		try {
			t = std::clamp(t, 0.0f, 1.0f);
			return interpolatePath(points, t).first;
		}
		catch (const std::exception& e) {
			throw Exceptions::TowernatorException("interpolatePosition failed: " + std::string(e.what()));
		}
	}

	sf::Vector2f GameToWindowCoords(sf::Vector2f coords, int gameTilemapHeight)
	{
		return static_cast<sf::Vector2f>(coords - sf::Vector2f(0.5f, 0.5f)) * (gameTilemapHeight / 9.0f);
	}

	float enemyToughnessMultiplierFunction(int roundNumber)
	{
		if (roundNumber < 1)
			throw Exceptions::TowernatorException("Invalid round number for toughness calculation");

		return std::max(log(roundNumber) / log(5), 1.0);
	}
}
