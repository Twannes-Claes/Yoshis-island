#pragma once
#include "utils.h"
#include "Texture.h"
#include "SoundEffect.h"

class CoinManager;
class BounceBallManager;
class Avatar;
class ShyGuyManager;
class TextureManager;
class SoundManager;
class SoundStream;

class Level final
{
	public:

		Level(TextureManager* textureManager, SoundManager* soundManager, const int volume);
		~Level();

		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;
		Level(Level&&) = delete;
		Level& operator=(Level&&) = delete;

		void Draw() const;
		void DrawFarBack() const;
		void DrawRaycast() const;

		void Update(float elapsedSec);

		void HandleObjectCollision(const Rectf& actorShape, const Rectf& projectileShape, const Vector2f& actorVelocity, float elapsedSec, float gravity, bool& isDead) const;

		void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, SoundEffect* flutterJump);
		bool IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity, float& jumpSpeed, const float& normalJumpSpeed) const;

		Rectf GetBoundaries() const;

		Point2f GetRespawnPoint() const;

		std::vector<Rectf> GetEnemyShapes() const;

		void RegulateMusicSound(const int volume);

		void RefreshLevel();

		bool HasReachedEnd(const Rectf& actorShape);

		bool CanZoom() const;

	private:

		std::vector<std::vector<Point2f>> m_Vertices{};

		const Texture* m_pLevelTexture;
		const Texture* m_pFarBackgroundTexture;
		const Texture* m_pGradientTexture;
		
		const SoundStream* m_pSong1;
		const SoundStream* m_pSongLevelClear;
		const SoundEffect* m_pYoshiCheerSound;

		float m_AccuLevelClear{};
		float m_DurationLevelClear{ 8.f };


		Rectf m_Boundaries;
		Point2f m_RespawnPoint;

		CoinManager* m_pCoinManager;
		BounceBallManager* m_pBounceBallManager;
		ShyGuyManager* m_pShyGuyManager;

		
		void AddObjects();
		void AddCoinsAndEnemies();

		bool m_IsOnGround{ false };
		bool m_IsMusicMuted{ false };
		bool m_IsLevelClearPlaying{ false };
		bool m_IsSongPlaying{ false };
		bool m_IsLevelCompleted{ false };

		Point2f m_RayTopVerticalLeft1{};
		Point2f m_RayTopVerticalLeft2{};
		//Top Right
		Point2f m_RayTopVerticalRight1{};
		Point2f m_RayTopVerticalRight2{};
		//Bottom
		Point2f m_RayBottomVerticalLeft1{};
		Point2f m_RayBottomVerticalLeft2{};

		Point2f m_RayBottomVerticalRight1{};
		Point2f m_RayBottomVerticalRight2{};
		//Horizontal Collission Variables
		// Left
		//Top
		Point2f m_RayRightTopHorizontal1{};
		Point2f m_RayRightTopHorizontal2{};
		//Bottom
		Point2f m_RayRightBotHorizontal1{};
		Point2f m_RayRightBotHorizontal2{};

		//Right
		//Top
		Point2f m_RayLeftTopHorizontal1{};
		Point2f m_RayLeftTopHorizontal2{};
		//Bottom
		Point2f m_RayLeftBotHorizontal1{};
		Point2f m_RayLeftBotHorizontal2{};

		Point2f m_RayMiddle1{};
		Point2f m_RayMiddle2{};
};

