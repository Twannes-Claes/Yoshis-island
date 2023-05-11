#include "pch.h"
#include "BounceBall.h"
#include "Texture.h"
#include "utils.h"
#include "Avatar.h"

BounceBall::BounceBall(const Point2f& bottomLeft, const TypeBall& type, const Texture* texture)
	:GameObject{ Rectf{} }
	,m_Type{type}
	,m_pTexture{ texture}
{
	if (type == TypeBall::big)
	{
		m_Shape.left = bottomLeft.x;
		m_Shape.bottom = bottomLeft.y;
		m_Shape.width = m_pTexture->GetWidth();
		m_Shape.height = m_pTexture->GetHeight();
	}
	if (type == TypeBall::small)
	{
		m_Shape.left = bottomLeft.x;
		m_Shape.bottom = bottomLeft.y;
		m_Shape.width = m_pTexture->GetWidth()/1.5f;
		m_Shape.height = m_pTexture->GetHeight() / 1.5f;
	}
	m_BallTopHitBox.push_back(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height));
	m_BallTopHitBox.push_back(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height));
}

void BounceBall::Draw() const
{

	glPushMatrix();
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0.f);
		Rectf dstRect{ -m_Shape.width / 2, 0.f, m_Shape.width, m_Shape.height };

		m_pTexture->Draw(dstRect);
	}
	glPopMatrix();
	//utils::DrawRect(m_Shape);

}
void BounceBall::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity)
{
	//std::cout << "on the hitbox\n";
	utils::HitInfo hitInfo{};
	Point2f rayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f rayP2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1 };

	if (actorVelocity.y <= 0.05f)
	{
		if (utils::Raycast(m_BallTopHitBox, rayP1, rayP2, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0.f;
		}
	}
}

bool BounceBall::IsOnGround(const Rectf& actorShape, const Vector2f actorVelocity, float& jumpSpeed)
{
	utils::HitInfo hitInfo{};
	Point2f rayP1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f rayP2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f };

	if (actorVelocity.y <= 0.05f)
	{
		if (utils::Raycast(m_BallTopHitBox, rayP1, rayP2, hitInfo))
		{
			if (m_Type == TypeBall::small)
			{
				jumpSpeed = m_JumpSpeedSmall;
			}
			else if (m_Type == TypeBall::big)
			{
				jumpSpeed = m_JumpSpeedBig;
			}
			return true;
		}
		return false;
	}
	return false;
}
