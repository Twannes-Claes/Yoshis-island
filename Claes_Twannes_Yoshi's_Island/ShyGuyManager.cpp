#include "pch.h"
#include "ShyGuyManager.h"
#include "utils.h"
#include "TextureManager.h"
#include "SoundManager.h"

ShyGuyManager::ShyGuyManager(TextureManager* textureManager, SoundManager* soundManager, const int volume)
	:m_pTexture{ textureManager->GetTexturePointer("Resources/Images/Level/ShyGuy/ShyGuy.png")}
	,m_pDeathSound{ soundManager->GetSoundPointer("Resources/Sounds/HitEnemy.wav", volume)}
{
}

ShyGuyManager::~ShyGuyManager()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
	}

	//delete m_pTexture;
	//m_pTexture = nullptr;
}

void ShyGuyManager::Draw() const
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Draw();
	}
}

void ShyGuyManager::Update(float elapsedSec, const float gravity, const std::vector<std::vector<Point2f>>& vertices)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		m_pItems[i]->Update(elapsedSec, gravity, vertices);
	}
}

ShyGuy* ShyGuyManager::AddItem(const Point2f& bottomLeft, const bool isFacingLeft, const float walkDistance)
{
	ShyGuy* item = new ShyGuy{ bottomLeft, m_pTexture, isFacingLeft, walkDistance };
	m_pItems.push_back(item);

	return item;
}

size_t ShyGuyManager::Size() const
{
	return m_pItems.size();
}

void ShyGuyManager::HandleExternalCollision(const Rectf& actorShape, const Rectf& projectileShape, const Vector2f& actorVelocity, const Rectf& boundaries, bool& isDead)
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		if (utils::IsOverlapping(m_pItems[i]->GetShape(), projectileShape) || m_pItems[i]->HandleTopCollision(actorShape, actorVelocity))
		{
			m_pItems[i]->GiveSmallDeathJumpBoost();
			m_pItems[i]->SetIsHitTrue();
			//m_pDeathSound->StopAll();
			m_pDeathSound->Play(0);
		}

		//kill player
		Rectf corePlayer{};
		corePlayer.left = actorShape.left + actorShape.width * 0.25f;
		corePlayer.bottom = actorShape.bottom + actorShape.height * 0.25f;
		corePlayer.width = actorShape.width * 0.5f; 
		corePlayer.height = actorShape.height * 0.5f;

		if (utils::IsOverlapping(m_pItems[i]->GetShape(), corePlayer) && !m_pItems[i]->GetIsHit())
		{
				isDead = true;
		}

		if (m_pItems[i]->IsOutsideBoundaries(boundaries))
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

std::vector<Rectf> ShyGuyManager::GetShapes() const
{
	std::vector<Rectf> enemyShapes;
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		enemyShapes.push_back( m_pItems[i]->GetShape());
	}
	return enemyShapes;
}

void ShyGuyManager::RefreshShyGuys()
{
	for (size_t i = 0; i < m_pItems.size(); i++)
	{
		delete m_pItems[i];
		m_pItems[i] = nullptr;
	}
	m_pItems.clear();
}
