#include "pch.h"
#include "Level.h"
#include "SVGParser.h"
#include "CoinManager.h"
#include "BounceBallManager.h"
#include "Avatar.h"
#include "ShyGuyManager.h"
#include "TextureManager.h"
#include "SoundStream.h"
#include "SoundManager.h"

Level::Level(TextureManager* textureManager, SoundManager* soundManager, const int volume)
	:m_pLevelTexture{ textureManager->GetTexturePointer("Resources/Images/Level/Level1/Level1.png") }
	,m_pFarBackgroundTexture{ textureManager->GetTexturePointer("Resources/Images/Level/Level1/BackgroundLvl1.png") }
	,m_pGradientTexture{ textureManager->GetTexturePointer("Resources/Images/Level/Level1/backgroundGradientLvl1.png") }
	,m_pCoinManager{ new CoinManager(textureManager, soundManager, volume) }
	,m_pBounceBallManager{ new BounceBallManager(textureManager) }
	,m_RespawnPoint{ Point2f(50.f, 125.f) }
	,m_pShyGuyManager{ new ShyGuyManager(textureManager, soundManager, volume) }
	,m_pSong1{new SoundStream("Resources/Sounds/Song1.mp3")}
	,m_pSongLevelClear{new SoundStream("Resources/Sounds/LevelClear.mp3")}
	,m_pYoshiCheerSound{soundManager->GetSoundPointer("Resources/Sounds/YoshiCheer.wav", volume)}
{
	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/Level1/Level1.svg", m_Vertices);
	m_Boundaries = Rectf(0.f, 0.f, m_pLevelTexture->GetWidth(), m_pLevelTexture->GetHeight());
	
	AddObjects();
	AddCoinsAndEnemies();
	/*m_pSong1->SetVolume(m_VolumeMusic);
	m_pSong1->Play(true);*/
}

Level::~Level()
{
	//delete m_pBackgroundTexture;
	//m_pBackgroundTexture = nullptr;

	delete m_pCoinManager;
	m_pCoinManager = nullptr;

	//delete m_pFarBackgroundTexture;
	//m_pFarBackgroundTexture = nullptr;

	//delete m_pGradientTexture;
	//m_pGradientTexture = nullptr;

	delete m_pBounceBallManager;
	m_pBounceBallManager = nullptr;

	delete m_pShyGuyManager;
	m_pShyGuyManager = nullptr;

	delete m_pSong1;
	m_pSong1 = nullptr;

	delete m_pSongLevelClear;
	m_pSongLevelClear = nullptr;
}

void Level::Draw() const
{
	m_pLevelTexture->Draw();

	m_pBounceBallManager->Draw();

	m_pCoinManager->Draw();

	m_pShyGuyManager->Draw();
}

void Level::DrawRaycast() const
{
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));

	utils::DrawLine(m_RayTopVerticalLeft1, m_RayTopVerticalLeft2);
	utils::DrawLine(m_RayTopVerticalRight1, m_RayTopVerticalRight2);

	utils::DrawLine(m_RayBottomVerticalLeft1, m_RayBottomVerticalLeft2);
	utils::DrawLine(m_RayBottomVerticalRight1, m_RayBottomVerticalRight2);

	utils::DrawLine(m_RayRightTopHorizontal1, m_RayRightTopHorizontal2);
	utils::DrawLine(m_RayRightBotHorizontal1, m_RayRightBotHorizontal2);

	utils::DrawLine(m_RayLeftTopHorizontal1, m_RayLeftTopHorizontal2);
	utils::DrawLine(m_RayLeftBotHorizontal1, m_RayLeftBotHorizontal2);

}

void Level::Update(float elapsedSec)
{
	if (!m_IsSongPlaying)
	{
		m_pSong1->Play(true);
		m_IsSongPlaying = true;
	}
	if (m_IsLevelClearPlaying)
	{
		if (m_AccuLevelClear >= m_DurationLevelClear)
		{
			m_IsLevelCompleted = true;
			m_pYoshiCheerSound->StopAll();
			m_pYoshiCheerSound->Play(0);
			m_AccuLevelClear = 0.f;
		}
		else
		{
			m_AccuLevelClear += elapsedSec;
		}
	}
}

void Level::DrawFarBack() const
{
	m_pGradientTexture->Draw(m_Boundaries);

	Rectf dstRect{
		0.f,
		0.f,
		m_pFarBackgroundTexture->GetWidth(),
		m_pFarBackgroundTexture->GetHeight()
	};

	int backgroundRepeats = int(m_pLevelTexture->GetWidth() / m_pFarBackgroundTexture->GetWidth()) + 1;
	for (int i{}; i < backgroundRepeats; ++i)
	{
		dstRect.left = m_pFarBackgroundTexture->GetWidth() * i;
		m_pFarBackgroundTexture->Draw(dstRect);
	}
}

void Level::HandleObjectCollision(const Rectf& actorShape, const Rectf& projectileShape, const Vector2f& actorVelocity, float elapsedSec, float gravity, bool& isDead) const
{
	m_pCoinManager->Update(actorShape, projectileShape, elapsedSec);
	m_pShyGuyManager->Update(elapsedSec, gravity, m_Vertices);
	m_pShyGuyManager->HandleExternalCollision(actorShape, projectileShape, actorVelocity, m_Boundaries, isDead);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, SoundEffect* flutterJump)
{
	utils::HitInfo hitInfo{}, hitInfo2{};

	//Vertical Collision Variables
	//Top Left
	const Point2f rayTopVerticalLeft1 = Point2f(actorShape.left + actorShape.width * 0.25f, actorShape.bottom + actorShape.height * 0.5f);
	const Point2f rayTopVerticalLeft2 = Point2f(actorShape.left + actorShape.width * 0.25f, actorShape.bottom + actorShape.height + 1);

	m_RayTopVerticalLeft1 = rayTopVerticalLeft1;
	m_RayTopVerticalLeft2 = rayTopVerticalLeft2;

	//Top Right
	const Point2f rayTopVerticalRight1 = Point2f(actorShape.left + actorShape.width * 0.75f , actorShape.bottom + actorShape.height * 0.5f);
	const Point2f rayTopVerticalRight2 = Point2f(actorShape.left + actorShape.width * 0.75f, actorShape.bottom + actorShape.height + 1);

	m_RayTopVerticalRight1 = rayTopVerticalRight1;
	m_RayTopVerticalRight2 = rayTopVerticalRight2;

	//Bottom
	const Point2f rayBottomVerticalLeft1 = Point2f(actorShape.left + actorShape.width * 0.25f, actorShape.bottom + actorShape.height * 0.5f);
	const Point2f rayBottomVerticalLeft2 = Point2f(actorShape.left + actorShape.width * 0.25f, actorShape.bottom - 1);

	m_RayBottomVerticalLeft1 = rayBottomVerticalLeft1;
	m_RayBottomVerticalLeft2 = rayBottomVerticalLeft2;

	const Point2f rayBottomVerticalRight1 = Point2f(actorShape.left + actorShape.width * 0.75f, actorShape.bottom + actorShape.height * 0.5f);
	const Point2f rayBottomVerticalRight2 = Point2f(actorShape.left + actorShape.width * 0.75f, actorShape.bottom - 1);

	m_RayBottomVerticalRight1 = rayBottomVerticalRight1;
	m_RayBottomVerticalRight2 = rayBottomVerticalRight2;

	//Horizontal Collission Variables
	// Left
	//Top
	const Point2f rayRightTopHorizontal1 = Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.75f);
	const Point2f rayRightTopHorizontal2 = Point2f(actorShape.left + actorShape.width * 0.9f, actorShape.bottom + actorShape.height * 0.75f);

	m_RayRightTopHorizontal1 = rayRightTopHorizontal1;
	m_RayRightTopHorizontal2 = rayRightTopHorizontal2;

	//Bottom
	const Point2f rayRightBotHorizontal1 = Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.25f);
	const Point2f rayRightBotHorizontal2 = Point2f(actorShape.left + actorShape.width * 0.75f, actorShape.bottom + actorShape.height * 0.25f);

	m_RayRightBotHorizontal1 = rayRightBotHorizontal1;
	m_RayRightBotHorizontal2 = rayRightBotHorizontal2;

	//Right
	//Top
	const Point2f rayLeftTopHorizontal1 = Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.75f);
	const Point2f rayLeftTopHorizontal2 = Point2f(actorShape.left + actorShape.width * 0.1f, actorShape.bottom + actorShape.height * 0.75f);
	
	m_RayLeftTopHorizontal1 = rayLeftTopHorizontal1;
	m_RayLeftTopHorizontal2 = rayLeftTopHorizontal2;

	//Bottom
	const Point2f rayLeftBotHorizontal1 = Point2f(actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height * 0.25f);
	const Point2f rayLeftBotHorizontal2 = Point2f(actorShape.left + actorShape.width * 0.25f, actorShape.bottom + actorShape.height * 0.25f);

	m_RayLeftBotHorizontal1 = rayLeftBotHorizontal1;
	m_RayLeftBotHorizontal2 = rayLeftBotHorizontal2;


	m_IsOnGround = false;
	
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		if (actorVelocity.y <= 0)
		{
			//RayCastFeet

			const bool isLeftHit = utils::Raycast(m_Vertices[i], rayBottomVerticalLeft1, rayBottomVerticalLeft2, hitInfo);
			const bool isRightHit = utils::Raycast(m_Vertices[i], rayBottomVerticalRight1, rayBottomVerticalRight2, hitInfo2);
			if (isLeftHit || isRightHit)
			{
				if (actorVelocity.y < -2.f)
				{
					flutterJump->StopAll();
				}
				actorVelocity.y = 0.f;


				if (isLeftHit && isRightHit)
				{
					if (actorVelocity.y < -2.f)
					{
						flutterJump->StopAll();
					}
					actorShape.bottom = hitInfo.intersectPoint.y > hitInfo2.intersectPoint.y ? hitInfo.intersectPoint.y : hitInfo2.intersectPoint.y;
				}
				else
				{
					if (actorVelocity.y < -2.f)
					{
						flutterJump->StopAll();
					}
					actorShape.bottom = isLeftHit ? hitInfo.intersectPoint.y : hitInfo2.intersectPoint.y;
				}

				m_IsOnGround = true;
			}
		}
		else
		{
			//RayCastHead
			if (utils::Raycast(m_Vertices[i], rayTopVerticalLeft1, rayTopVerticalLeft2, hitInfo) ||
				utils::Raycast(m_Vertices[i], rayTopVerticalRight1, rayTopVerticalRight2, hitInfo))
			{
				actorVelocity.y = -1.f;
			}
		}

		//RayCastRight Top
		if (utils::Raycast(m_Vertices[i], rayRightTopHorizontal1, rayRightTopHorizontal2, hitInfo) ||
			utils::Raycast(m_Vertices[i], rayRightBotHorizontal1, rayRightBotHorizontal2, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - (actorShape.width*0.9f);
			actorVelocity.x = 0.f;
		}


		//RayCastLeft
		if (utils::Raycast(m_Vertices[i], rayLeftTopHorizontal1, rayLeftTopHorizontal2, hitInfo) ||
			utils::Raycast(m_Vertices[i], rayLeftBotHorizontal1, rayLeftBotHorizontal2, hitInfo))
		{
			actorShape.left = hitInfo.intersectPoint.x - (actorShape.width*0.1f);
			actorVelocity.x = 0.f;
		}
	}
	//Handle collision for walking on the ball
	m_pBounceBallManager->HandleCollision(actorShape, actorVelocity);
}

bool Level::IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity, float& jumpSpeed, const float& normalJumpSpeed) const
{
	for (size_t i{}; i < m_Vertices.size(); ++i)
	{
		if (m_IsOnGround)
		{
			jumpSpeed = normalJumpSpeed;
			//when transitioning check if it can play music when done easy check if character is on ground
			
			return true;
		}
		if (m_pBounceBallManager->IsOnGround(actorShape, actorVelocity, jumpSpeed))
		{
			
			return true;
		}
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

Point2f Level::GetRespawnPoint() const
{
	return m_RespawnPoint;
}

std::vector<Rectf> Level::GetEnemyShapes() const
{
	return m_pShyGuyManager->GetShapes();
}

void Level::RegulateMusicSound(const int volume)
{
	m_pSong1->SetVolume(volume);
}

void Level::RefreshLevel()
{
	m_pCoinManager->RefreshCoins();
	m_pShyGuyManager->RefreshShyGuys();
	AddCoinsAndEnemies();
	m_IsSongPlaying = false;
	m_IsLevelClearPlaying = false;
	m_IsLevelCompleted = false;
}

bool Level::HasReachedEnd(const Rectf& actorShape)
{
	if (actorShape.left + actorShape.width >= m_Boundaries.width - 1.f)
	{
		if (!m_IsLevelClearPlaying)
		{
			m_pSong1->Stop();
			m_pSongLevelClear->Play(false);
			m_IsLevelClearPlaying = true;
		}	
	}

	if (m_IsLevelCompleted)
	{
		return true;
	}

	return false;
}

bool Level::CanZoom() const
{
	return m_IsLevelClearPlaying;
}

void Level::AddObjects()
{
	
	m_pBounceBallManager->AddItem(Point2f(1244.f, 432.f), BounceBall::TypeBall::big);
	m_pBounceBallManager->AddItem(Point2f(1104.f, 320.f), BounceBall::TypeBall::small);
}

void Level::AddCoinsAndEnemies()
{
	m_pCoinManager->AddItem(Point2f{ 274.f, 193.f });
	m_pCoinManager->AddItem(Point2f{ 306.f, 193.f });
	m_pCoinManager->AddItem(Point2f{ 338.f, 193.f });
	m_pCoinManager->AddItem(Point2f{ 498.f, 257.f });
	m_pCoinManager->AddItem(Point2f{ 530.f, 257.f });
	m_pCoinManager->AddItem(Point2f{ 498.f, 305.f });
	m_pCoinManager->AddItem(Point2f{ 530.f, 305.f });
	m_pCoinManager->AddItem(Point2f{ 626.f, 272.f });
	m_pCoinManager->AddItem(Point2f{ 658.f, 304.f });
	m_pCoinManager->AddItem(Point2f{ 690.f, 304.f });
	m_pCoinManager->AddItem(Point2f{ 722.f, 288.f });
	m_pCoinManager->AddItem(Point2f{ 850.f, 288.f });
	m_pCoinManager->AddItem(Point2f{ 866.f, 304.f });
	m_pCoinManager->AddItem(Point2f{ 898.f, 320.f });
	m_pCoinManager->AddItem(Point2f{ 930.f, 320.f });
	m_pCoinManager->AddItem(Point2f{ 1106.f, 368.f });
	m_pCoinManager->AddItem(Point2f{ 1106.f, 400.f });
	m_pCoinManager->AddItem(Point2f{ 1106.f, 432.f });
	m_pCoinManager->AddItem(Point2f{ 1122.f, 464.f });
	m_pCoinManager->AddItem(Point2f{ 1150.f, 464.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 496.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 528.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 560.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 592.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 624.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 656.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 688.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 720.f });
	m_pCoinManager->AddItem(Point2f{ 1250.f, 752.f });
	m_pCoinManager->AddItem(Point2f{ 1266.f, 784.f });
	m_pCoinManager->AddItem(Point2f{ 1290.f, 790.f });
	m_pCoinManager->AddItem(Point2f{ 1305.f, 770.f });
	m_pShyGuyManager->AddItem(Point2f(315.f, 125.f), true, 50.f);
	m_pShyGuyManager->AddItem(Point2f(1400.f, 726.f), true, 50.f);
	m_pShyGuyManager->AddItem(Point2f(1450.f, 726.f), false, 50.f);
}
