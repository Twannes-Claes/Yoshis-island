#include "pch.h"
#include "BounceBallManager.h"
#include "Texture.h"
#include "TextureManager.h"

BounceBallManager::BounceBallManager(TextureManager* textureManager)
	:m_pTexture{ textureManager->GetTexturePointer("Resources/Images/Level/BounceBall/BounceBall.png")}
{
}

BounceBallManager::~BounceBallManager()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}

	//delete m_pTexture;
	//m_pTexture = nullptr;
}

BounceBall* BounceBallManager::AddItem(const Point2f& botLeft, const BounceBall::TypeBall& type)
{
	BounceBall* item = new BounceBall{ botLeft, type, m_pTexture};
	m_pItems.push_back(item);
	return item;
}
void BounceBallManager::Update(const Rectf& actorShape, float elapsedSec)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
	}
}
void BounceBallManager::Draw() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Draw();
	}
}
void BounceBallManager::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->HandleCollision(actorShape, actorVelocity);
	}
}
bool BounceBallManager::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity, float& jumpSpeed)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{	
		if (m_pItems[i]->IsOnGround(actorShape, actorVelocity, jumpSpeed))
		{
			return true;
		}
		
	}
	return false;
}
size_t BounceBallManager::Size() const
{
	return m_pItems.size();
}