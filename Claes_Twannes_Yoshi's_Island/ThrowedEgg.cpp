#include "pch.h"
#include "ThrowedEgg.h"
#include "Texture.h"
#include "Level.h"
#include <iostream>
#include "utils.h"

ThrowedEgg::ThrowedEgg(const Point2f& pos, const Vector2f& velocity, const Texture* texture)
	:GameObject{ Rectf{} }
	,m_Velocity{velocity}
	,m_pTexture{ texture }
{
	m_Shape.left = pos.x;
	m_Shape.bottom = pos.y - m_pTexture->GetHeight()/4;
	m_Shape.width = m_pTexture->GetWidth() / 2;
	m_Shape.height = m_pTexture->GetHeight() / 2;
}

void ThrowedEgg::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_Shape.left + m_Shape.width, m_Shape.bottom, 0.f);
		Rectf dstRect{ -m_Shape.width, 0.f, m_Shape.width, m_Shape.height };
		m_pTexture->Draw(dstRect);
	}
	glPopMatrix();
	//utils::DrawRect(m_Shape);
}

void ThrowedEgg::Update(float elapsedSec, float gravity)
{
	m_Shape.left += (m_Velocity.x * 4) * elapsedSec;
	m_Shape.bottom += (m_Velocity.y * 4) * elapsedSec;
	m_Velocity.y += (gravity/15) * elapsedSec;
}

bool ThrowedEgg::IsOutsideBoundaries(const Level& level) const
{
	if (m_Shape.left + m_Shape.width <= level.GetBoundaries().left)
	{
		return true;
	}
	else if (m_Shape.left >= level.GetBoundaries().left + level.GetBoundaries().width)
	{
		return true;
	}
	if (m_Shape.bottom + m_Shape.height <= level.GetBoundaries().bottom)
	{
		return true;
	}
	else if (m_Shape.bottom >= level.GetBoundaries().bottom + level.GetBoundaries().height)
	{
		return true;
	}
	return false;
}
