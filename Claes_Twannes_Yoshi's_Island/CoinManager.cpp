#include "pch.h"
#include "CoinManager.h"
#include <iostream>
#include "Texture.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SoundEffect.h"

CoinManager::CoinManager(TextureManager* textureManager, SoundManager* soundManager, const int volume)
	:m_Effect{soundManager->GetSoundPointer("Resources/Sounds/Coin.wav", volume)}
	,m_pTextureCoin{ textureManager->GetTexturePointer("Resources/Images/Level/Coin/Coin.png")}
	,m_pTextureEffect{ textureManager->GetTexturePointer("Resources/Images/Level/Coin/CoinEffect.png") }
{
}
CoinManager::~CoinManager()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
	}
	//delete m_pTextureCoin;
	//m_pTextureCoin = nullptr;
	//delete m_pTextureEffect;
	//m_pTextureEffect= nullptr;
}
Coin* CoinManager::AddItem(const Point2f& botLeft)
{
	Coin* item = new Coin{ botLeft, m_pTextureCoin, m_pTextureEffect};
	m_pItems.push_back(item);

	return item;
}
void CoinManager::Update(const Rectf& actorShape, const Rectf& projectileShape, float elapsedSec)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Update(elapsedSec);

		if (m_pItems[i]->IsOverlapping(actorShape, projectileShape) && !m_pItems[i]->GetIsPickedUp())
		{
			m_pItems[i]->SetIsPickedUp();
			m_Effect->Play(0);
		}
		if (m_pItems[i]->GetIsPickedUp())
		{
			if (m_pItems[i]->IsAbleToDelete())
			{
				delete m_pItems[i];
				m_pItems[i] = nullptr;

				if (i != m_pItems.size() - 1)
				{
					m_pItems[i] = m_pItems[m_pItems.size() - 1];
				}
				m_pItems.pop_back();
			}
		}
	}
}
void CoinManager::Draw() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Draw();
	}
}
size_t CoinManager::Size() const
{
	return m_pItems.size();
}

void CoinManager::RefreshCoins()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}
	m_pItems.clear();
}
