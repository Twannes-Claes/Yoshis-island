#pragma once
#include "GameObject.h"
#include <vector>
#include "Vector2f.h"

class ShyGuy final: public GameObject 
{
	public:

		ShyGuy(const Point2f& bottomLeft, const Texture* texture, const bool isFacingLeft, const float walkDistance);
		~ShyGuy() = default;

		void Draw() const;
		void Update(float elapsedSec, const float gravity, const std::vector<std::vector<Point2f>> vertices);

		ShyGuy(const ShyGuy&) = delete;
		ShyGuy& operator=(const ShyGuy&) = delete;
		ShyGuy(ShyGuy&&) = delete;
		ShyGuy& operator=(ShyGuy&&) = delete;

		void SetIsHitTrue();
		bool GetIsHit() const;
		void GiveSmallDeathJumpBoost();
		bool IsOutsideBoundaries(const Rectf& level) const;
		bool HandleTopCollision(const Rectf& actorShape, const Vector2f& actorVelocity);

	private:

		std::vector<Point2f> m_TopHitBox;

		const int m_NrOffFrames{5};
		const int m_NrFramesPerSec{8};
		float m_AnimTime{};
		int m_AnimFrame{};
		const int m_Columns{5};

		float m_IdleTimer{};
		const float m_IdleTime{1.f};

		bool m_IsFacingLeft;
		bool m_IsMoving{ true };
		bool m_IsHit{false};

		const float m_WalkDistance;
		float m_LeftWalkBorder;
		float m_RightWalkBorder;
		Vector2f m_Velocity{ 0.f, 0.f };
		const float m_WalkSpeed{ 50.f };

		const Texture* m_pTexture;

		void UpdateAnimation(float elapsedSec);
		void UpdateMoving(float elapsedSec);
		void UpdateIdle(float elapsedSec);
		void HandleGroundCollision(const std::vector<std::vector<Point2f>>& vertices, const float gravity, float elapsedSec);
		
};

