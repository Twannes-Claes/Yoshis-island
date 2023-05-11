#pragma once
#include <vector>
#include "BounceBall.h"

class BounceBall;
class TextureManager;

class BounceBallManager final
{
	public:

		BounceBallManager(TextureManager* textureManager);
		~BounceBallManager();

		BounceBallManager(const BounceBallManager&) = delete;
		BounceBallManager& operator=(const BounceBallManager&) = delete;
		BounceBallManager(BounceBallManager&&) = delete;
		BounceBallManager& operator=(BounceBallManager&&) = delete;

		
		void Update(const Rectf& actorShape, float elapsedSec);
		void Draw() const;

		void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity);
		bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity, float& jumpSpeed);

		BounceBall* AddItem(const Point2f& botLeft, const BounceBall::TypeBall& type);

		size_t Size() const;

	private:

		std::vector<BounceBall*> m_pItems{};

		const Texture* m_pTexture;
};

