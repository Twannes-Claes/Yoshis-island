#include "pch.h"
#include "ShyGuy.h"
#include "Texture.h"
#include  "utils.h"
#include <iostream>

ShyGuy::ShyGuy(const Point2f& bottomLeft, const Texture* texture, const bool isFacingLeft, const float walkDistance)
	:GameObject{ Rectf{} }
	,m_pTexture{texture}
	,m_IsFacingLeft{isFacingLeft}
	,m_WalkDistance{walkDistance}
{
	m_Shape.bottom = bottomLeft.y;
	m_Shape.left = bottomLeft.x;
	m_Shape.width = texture->GetWidth()/m_Columns;
	m_Shape.height = texture->GetHeight();

	if (isFacingLeft)
	{
		m_LeftWalkBorder = bottomLeft.x - m_WalkDistance;
		m_RightWalkBorder = bottomLeft.x + m_WalkDistance;
	}
	else
	{
		m_LeftWalkBorder = bottomLeft.x - m_WalkDistance;
		m_RightWalkBorder = bottomLeft.x + m_WalkDistance;
	}
	m_TopHitBox.push_back(Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height));
	m_TopHitBox.push_back(Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height));
}

void ShyGuy::Draw() const
{
	Rectf srcRect{};
	srcRect.width = m_Shape.width;
	srcRect.height = m_Shape.height;
	srcRect.left = srcRect.left = (m_AnimFrame % m_Columns) * srcRect.width;
	srcRect.bottom = srcRect.height;

	glPushMatrix();
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2, m_Shape.bottom, 0.f);
		if (!m_IsFacingLeft)
		{
			glScalef(-1.f, 1.f, 1.f);
		}
		Rectf dstRect{ -m_Shape.width / 2, 0.f, m_Shape.width, m_Shape.height };

		m_pTexture->Draw(dstRect, srcRect);
	}
	glPopMatrix();
	//utils::SetColor(Color4f(0.f, 0.f, 0.f, 1.f));
	//utils::DrawRect(m_Shape);
	//utils::DrawLine(m_TopHitBox[0], m_TopHitBox[1]);
}

void ShyGuy::Update(float elapsedSec, const float gravity, const std::vector<std::vector<Point2f>> vertices)
{
	HandleGroundCollision(vertices, gravity, elapsedSec);

	UpdateAnimation(elapsedSec);

	UpdateIdle(elapsedSec);

	UpdateMoving(elapsedSec);
}

void ShyGuy::SetIsHitTrue()
{
	m_IsHit = true;
}

bool ShyGuy::GetIsHit() const
{
	return m_IsHit;
}

void ShyGuy::GiveSmallDeathJumpBoost()
{
	if (!m_IsHit)
	{
		m_Velocity.y = 200.f;
	}
}

void ShyGuy::UpdateAnimation(float elapsedSec)
{
	if (!m_IsMoving)
	{
		m_AnimFrame = 0;
	}
	else
	{
		m_AnimTime += elapsedSec;
		if (m_AnimTime > 1.f / m_NrFramesPerSec)
		{


			++m_AnimFrame %= m_NrOffFrames;
			m_AnimTime -= 1.f / m_NrFramesPerSec;

		}
	}
}

void ShyGuy::UpdateMoving(float elapsedSec)
{
	if (m_IsMoving && !m_IsHit)
	{
		if (m_IsFacingLeft)
		{
			m_Velocity.x = -m_WalkSpeed;

			if (m_Shape.left <= m_LeftWalkBorder)
			{
				m_IsMoving = false;
			}
		}
		else
		{
			m_Velocity.x = m_WalkSpeed;

			if (m_Shape.left >= m_RightWalkBorder)
			{
				m_IsMoving = false;
			}
		}
		//m_Shape.bottom += m_Velocity.y * elapsedSec;
		m_Shape.left += m_Velocity.x * elapsedSec;

		
	}
}

void ShyGuy::UpdateIdle(float elapsedSec)
{
	if (!m_IsMoving)
	{
		m_IdleTimer += elapsedSec;
		if (m_IdleTimer >= m_IdleTime)
		{
			m_IsFacingLeft = !m_IsFacingLeft;
			m_IsMoving = true;
			m_IdleTimer = 0.f;
		}
	}
}

void ShyGuy::HandleGroundCollision(const std::vector<std::vector<Point2f>>& vertices, const float gravity, float elapsedSec)
{
	if (!m_IsHit)
	{
		utils::HitInfo hitInfo{};

		Point2f rayBottomVertical1{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height * 0.12f };
		Point2f rayBottomVertical2{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom - 1 };

		for (size_t i{}; i < vertices.size(); ++i)
		{
			//RayCastFeet
			if (utils::Raycast(vertices[i], rayBottomVertical1, rayBottomVertical2, hitInfo))
			{
				m_Velocity.y = 0.f;
				m_Shape.bottom = hitInfo.intersectPoint.y;
			}
			else
			{
				m_Velocity.y += gravity / 200 * elapsedSec;
				m_Shape.bottom += m_Velocity.y * elapsedSec;
			}
		}
	}
	else
	{
		m_Velocity.y += gravity / 2 * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}
}

bool ShyGuy::HandleTopCollision(const Rectf& actorShape, const Vector2f& actorVelocity)
{
	if (!m_IsHit && actorVelocity.y < 0.f)
	{
		utils::HitInfo hitInfo{};

		m_TopHitBox[0] = (Point2f(m_Shape.left, m_Shape.bottom + m_Shape.height));
		m_TopHitBox[1] = (Point2f(m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height));

		Point2f rayBottomVerticalMiddle1{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.12f };
		Point2f rayBottomVerticalMiddle2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1 };

		Point2f rayBottomVerticalLeft1{ actorShape.left + actorShape.width / 4, actorShape.bottom + actorShape.height * 0.12f };
		Point2f rayBottomVerticalLeft2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1 };

		Point2f rayBottomVerticalRight1{ actorShape.left + actorShape.width * 0.75f, actorShape.bottom + actorShape.height * 0.12f };
		Point2f rayBottomVerticalRight2{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1 };
		//RayCastFeet
		if (utils::Raycast(m_TopHitBox, rayBottomVerticalMiddle1, rayBottomVerticalMiddle2, hitInfo))
		{
			m_IsHit = true;
			return true;
		}
		if (utils::Raycast(m_TopHitBox, rayBottomVerticalLeft1, rayBottomVerticalLeft2, hitInfo))
		{
			m_IsHit = true;
			return true;
		}
		if (utils::Raycast(m_TopHitBox, rayBottomVerticalRight1, rayBottomVerticalRight2, hitInfo))
		{
			m_IsHit = true;
			return true;
		}
	}
	return false;
}

bool ShyGuy::IsOutsideBoundaries(const Rectf& boundaries) const
{
	if (m_Shape.bottom <= boundaries.bottom)
	{
		return true;
	}
	return false;
}
