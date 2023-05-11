#pragma once
#include <vector>
#include "ThrowedEgg.h"

class TextureManager;

class ThrowedEggManager final
{
	public:
		ThrowedEggManager(TextureManager* textureManager);
		~ThrowedEggManager();

		ThrowedEggManager(const ThrowedEggManager&) = delete;
		ThrowedEggManager& operator=(const ThrowedEggManager&) = delete;
		ThrowedEggManager(ThrowedEggManager&&) = delete;
		ThrowedEggManager& operator=(ThrowedEggManager&&) = delete;

		void Draw() const;
		void Update(float elapsedSec, float gravity, const Level& level, const std::vector<Rectf>& enemyShape);

		ThrowedEgg* AddItem(const Point2f& botLeft, const Vector2f& velocity);

		Rectf GetEggBox() const;

		size_t Size() const;

	private:

		std::vector<ThrowedEgg*> m_pItems{};

		const Texture* m_pTexture;

		Rectf m_EnemyShape{};

};

