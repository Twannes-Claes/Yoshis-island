#include "pch.h"
#include "ThrowedEggManager.h"
#include "Texture.h"
#include "utils.h"
#include "TextureManager.h"

ThrowedEggManager::ThrowedEggManager(TextureManager* textureManager)
{
	m_pTexture = textureManager->GetTexturePointer("Resources/Images/Avatar/ThrowingEgg/ThrowingEgg.png");
}

ThrowedEggManager::~ThrowedEggManager()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
	}
	//delete m_pTexture;
	//m_pTexture = nullptr;
}

void ThrowedEggManager::Draw() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Draw();
		utils::DrawRect(m_EnemyShape);
	}

	
}

void ThrowedEggManager::Update(float elapsedSec, float gravity, const Level& level, const std::vector<Rectf>& enemyShape)
{
	bool hitByEgg{};
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		bool isDestroyed{};
		for (int j{}; j < enemyShape.size(); ++j)
		{
			Rectf coreEnemy{};
			coreEnemy.left = enemyShape[j].left + enemyShape[j].width * 0.05f;
			coreEnemy.bottom = enemyShape[j].bottom + enemyShape[j].height * 0.05f;
			coreEnemy.width = enemyShape[j].width * 0.9f;
			coreEnemy.height = enemyShape[j].height * 0.9f;

			if (utils::IsOverlapping(m_pItems[i]->GetShape(), coreEnemy))
			{
				hitByEgg = true;
			}		
		}

		if (m_pItems[i]->IsOutsideBoundaries(level) || hitByEgg)
		{
			delete m_pItems[i];
			m_pItems[i] = nullptr;
			isDestroyed = true;

			if (i != m_pItems.size() - 1)
			{
				m_pItems[i] = m_pItems[m_pItems.size() - 1];
			}
			m_pItems.pop_back();
		}

		if (!isDestroyed)
		{
			m_pItems[i]->Update(elapsedSec, gravity);
		}
	}
}

ThrowedEgg* ThrowedEggManager::AddItem(const Point2f& botLeft, const Vector2f& velocity)
{
	ThrowedEgg* item = new ThrowedEgg{ botLeft, velocity, m_pTexture};
	m_pItems.push_back(item);

	return item;
}

Rectf ThrowedEggManager::GetEggBox() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		return m_pItems[i]->GetShape();;
	}
	return Rectf{};
}

size_t ThrowedEggManager::Size() const
{
	return m_pItems.size();
}

