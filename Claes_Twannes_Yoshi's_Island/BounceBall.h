#pragma once
#include "Vector2f.h"
#include <vector>
#include "GameObject.h"

class Texture;
class Avatar;

class BounceBall final: public GameObject
{
	public:

		enum class TypeBall
		{
			small = 0,
			big,
		};

		explicit BounceBall(const Point2f& bottomLeft, const TypeBall& type, const Texture* texture);
		~BounceBall() = default;

		BounceBall(const BounceBall&) = delete;
		BounceBall& operator=(const BounceBall&) = delete;
		BounceBall(BounceBall&&) = delete;
		BounceBall& operator=(BounceBall&&) = delete;

		void Draw() const;

		void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity);
		bool IsOnGround(const Rectf& actorShape, const Vector2f actorVelocity, float& jumpSpeed);

	private:

		std::vector<Point2f> m_BallTopHitBox;

		const float m_JumpSpeedSmall{ 475.f };
		const float m_JumpSpeedBig{ 775.f };

		const TypeBall m_Type;

		const Texture* m_pTexture;
};

