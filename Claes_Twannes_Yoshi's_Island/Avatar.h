#pragma once
#include "utils.h"
#include "Vector2f.h"
#include <vector>
#include "ThrowedEggManager.h"
#include "GameObject.h"

class Level;
class Texture;
class SoundEffect;
class TextureManager;
class SoundManager;
class HUD;

class Avatar final: public GameObject
{
	public:

		enum class AnimationState
		{
			yoshiMarioIdle = 0,
			yoshiMarioRun,
			yoshiMarioSprint,
			yoshiMarioJump,
			yoshiMarioClimbJump,
			yoshiMarioDuck,
			yoshiMarioAiming,
			yoshiMarioShooting,
			yoshiMarioDying
		};

		Avatar(const Point2f& respawnPoint, TextureManager* textureManager, SoundManager* soundManager, const int volume);
		~Avatar();

		Avatar(const Avatar&) = delete;
		Avatar& operator=(const Avatar&) = delete;
		Avatar(Avatar&&) = delete;
		Avatar& operator=(Avatar&&) = delete;

		void Update(float elapsedSec, /*const*/ Level& level, const HUD& hud);
		void Draw() const;

		Rectf GetEggBox() const;

		private:

		AnimationState m_AnimationState{ AnimationState::yoshiMarioIdle };

		//Rectf m_Shape{ /*1400.f, 750.f*/ 50.f, 125.f, 0.f, 0.f };

		float m_HorSpeed{};
		const float m_NormalHorSpeed{ 110.f };
		float m_JumpSpeed{};
		const float m_NormalJumpSpeed{ 360.f };

		Vector2f m_Velocity{ 0.f, 0.f };
		const float m_Gravity{ -981.f };

		std::vector<Texture*> m_pAnimationStateTextures;
	
		float m_ClipHeight{ 33.f };
		float m_ClipWidth{ 35.f };
		int m_NrOffFrames{ 30 };
		int m_NrFramesPerSec{7};
		float m_AnimTime{};
		int m_AnimFrame{};
		int m_Columns{5};

		float m_SprintAccuTime{};
		const float m_SprintTime{1.f};
		float m_JumpClimbAccuTime{};
		const float m_JumpClimbTime{ 1.8f };
		const float m_ClimbingSpeed{ 1015.f };
		const float m_LongFallSpeed{ -300.f };

		bool m_IsFacingLeft{ false };
		bool m_CanSprint{false};
		bool m_CanTimeSprint{ false };
		bool m_CanClimbJump{ false };
		bool m_IsDead{ false };
		bool m_FrameResetted{ false };
		bool m_IsHitSoundPlayed{ false };
		bool m_CanCrouchSoundBePlayed{ false };
		bool m_CanGameBeResetted{ false };

		const Texture* m_pCrossHairTexture;

		const SoundEffect* m_pJumpSound;
		SoundEffect* m_FlutterJumpSound;
		const SoundEffect* m_pBounceBallJumpSound;
		const SoundEffect* m_pAimingSound;
		const SoundEffect* m_pEggShotSound;
		const SoundEffect* m_pYoshiHitSound;
		const SoundEffect* m_pCrouchSound;

		Rectf m_CrossHairBox;
		float m_CrossHairAngle{};
		float m_CrossHairAngleSpeed{125.f};
		const float m_CrossHairOffSet{ 75.f };
		const float m_MaxAngle{ 75.f };
		const float m_MinAngle{ -40.f };
		
		ThrowedEggManager* m_pThrowedEggManager;

		void HandleKeyStates(float elapsedSec, bool isOnGround);
		void HandlePhysics(float elapsedSec);
		void UpdateAnimationFrames(float elapsedSec);
		void DrawAnimation() const;
		void Clamp(const Level& level);
		void ResetAnimation();
		void UpdateHitBox();
		void UpdateSprint(float elapsedSec);
		void UpdateJump(float elapsedSec, const Level& level);
		void UpdateCrossHair(float elapsedSec);
		Point2f ReturnCrossHairLocations();
		void Respawn(const Point2f& respawnPoint, Level& level, const HUD& hud);
};

