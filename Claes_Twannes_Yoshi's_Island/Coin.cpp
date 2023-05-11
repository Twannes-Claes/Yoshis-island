#include "pch.h"
#include "Coin.h"
#include "utils.h"
#include "Texture.h"
#include <iostream>
#include <gl/GL.h>

Coin::Coin(const Point2f& botLeft, const Texture* textureCoin, const Texture* textureEffect)
	:GameObject{ Rectf{} }
{
	m_pTextureCoin = textureCoin;
	m_Shape.bottom = botLeft.y;
	m_Shape.left = botLeft.x;
	m_Shape.width = m_ClipWidth;
	m_Shape.height = m_ClipHeight;

	m_pTextureCoinEffect = textureEffect;
	m_Shape.bottom = botLeft.y;
	m_Shape.left = botLeft.x;
	m_Shape.width = m_ClipWidth;
	m_Shape.height = m_ClipHeight;
}

void Coin::Update(float elapsedSec)
{
	if (!m_IsPickedUp)
	{
		m_NrOffFrames = 4;
		m_NrFramesPerSec = 3;
		m_ClipWidth = 12.f;
		m_ClipHeight = 16.f;
		m_Columns = 4;
	}
	else
	{
		m_EffectTimer += elapsedSec;
		m_NrOffFrames = 13;
		m_NrFramesPerSec = 6;
		m_ClipWidth = 35.f;
		m_ClipHeight = 21.f;
		m_Columns = 7;
	}
	m_AnimTime += elapsedSec;
	if (m_AnimTime > 1.f / m_NrFramesPerSec)
	{
		if (!m_IsPickedUp)
		{
			++m_AnimFrame %= m_NrOffFrames;
		}
		else
		{
			++m_AnimFrame;
			if (m_AnimFrame == 12)
			{
				m_IsAbleToDelete = true;
			}
		}
		m_AnimTime -= 1.f / m_NrFramesPerSec;
	}
}

void Coin::Draw() const
{
	Rectf srcRect{};
	srcRect.width = m_ClipWidth;
	srcRect.height = m_ClipHeight;
	srcRect.left = (m_AnimFrame % m_Columns) * srcRect.width;
	srcRect.bottom = (m_AnimFrame / m_Columns + 1) * srcRect.height;

	glPushMatrix();
		glTranslatef(m_Shape.left + m_Shape.width/2, m_Shape.bottom,0.f);
		Rectf dstRect{ -m_Shape.width / 2, 0.f, m_Shape.width, m_Shape.height };
		if (!m_IsPickedUp)
		{
			m_pTextureCoin->Draw(dstRect, srcRect);
		}
		else
		{
			m_pTextureCoinEffect->Draw(dstRect, srcRect);
		}
	glPopMatrix();
//utils::DrawRect(m_Shape);
}

bool Coin::IsOverlapping(const Rectf& actorShape, const Rectf& projectileShape) const
{
		return utils::IsOverlapping(actorShape, m_Shape) || utils::IsOverlapping(projectileShape, m_Shape);
}

void Coin::SetIsPickedUp()
{
	m_IsPickedUp = true;
}

bool Coin::GetIsPickedUp() const
{
	return m_IsPickedUp;
}

bool Coin::IsAbleToDelete() const
{
	return m_IsAbleToDelete;
}
