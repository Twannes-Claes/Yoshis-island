#include "pch.h"
#include "Avatar.h"
#include <iostream>
#include "Level.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "SoundEffect.h"
#include "HUD.h"

Avatar::Avatar(const Point2f& respawnPoint, TextureManager* textureManager, SoundManager* soundManager, const int volume)
	:GameObject{ Rectf{respawnPoint.x, respawnPoint.y, 0.f, 0.f} }
	,m_pCrossHairTexture{ textureManager->GetTexturePointer("Resources/Images/Avatar/CrossHair.png")}
	,m_pJumpSound{soundManager->GetSoundPointer("Resources/Sounds/Jump.wav", volume)}
	,m_FlutterJumpSound{soundManager->GetSoundPointer("Resources/Sounds/FlutterJump.wav", volume)}
	,m_pBounceBallJumpSound{soundManager->GetSoundPointer("Resources/Sounds/BounceBall.wav", volume)}
	,m_pAimingSound{soundManager->GetSoundPointer("Resources/Sounds/Aiming.wav", volume)}
	,m_pEggShotSound{soundManager->GetSoundPointer("Resources/Sounds/EggShoot.wav", volume)}
	,m_pYoshiHitSound{soundManager->GetSoundPointer("Resources/Sounds/YoshiHit.wav", volume)}
	,m_pThrowedEggManager{ new ThrowedEggManager(textureManager)}
	,m_pCrouchSound{soundManager->GetSoundPointer("Resources/Sounds/Crouch.wav", volume)}
{
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioIdle.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioRun.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioSprint.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioJump.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioClimbJump.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioDuck.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioAiming.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioAiming.png"));
	m_pAnimationStateTextures.push_back(textureManager->GetTexturePointer("Resources/Images/Avatar/YoshiMarioDying.png"));

	m_CrossHairBox.width = m_pCrossHairTexture->GetWidth() / 2;
	m_CrossHairBox.height = m_pCrossHairTexture->GetHeight();
	m_CrossHairBox.left = 0.f;
	m_CrossHairBox.bottom = 0.f;
}

Avatar::~Avatar()
{
	/*for (size_t i = 0; i < m_pAnimationStateTextures.size(); i++)
	{
		delete m_pAnimationStateTextures[i]; 
		m_pAnimationStateTextures[i] = nullptr;
	}*/

	//delete m_pCrossHairTexture;
	//m_pCrossHairTexture = nullptr;

	delete m_pThrowedEggManager;
	m_pThrowedEggManager = nullptr;
}

void Avatar::Update(float elapsedSec, /*const*/ Level& level, const HUD& hud)
{
	if (hud.GetGameState() == HUD::GameState::transitionToLevel)
	{
		Respawn(level.GetRespawnPoint(), level, hud);
		m_CanGameBeResetted = false;
	}


	if (hud.GetGameState() == HUD::GameState::playingLevel)
	{
		
		if (level.IsOnGround(m_Shape, m_Velocity, m_JumpSpeed, m_NormalJumpSpeed))
		{
			m_Velocity.y = 0;
			m_CanClimbJump = true;
			HandleKeyStates(elapsedSec, true);
		}
		else
		{
			m_Velocity.y += m_Gravity * elapsedSec;
			HandleKeyStates(elapsedSec, false);
		}
	}

	HandlePhysics(elapsedSec);

	level.HandleObjectCollision(m_Shape, GetEggBox(), m_Velocity, elapsedSec, m_Gravity, m_IsDead);

	level.HandleCollision(m_Shape, m_Velocity, m_FlutterJumpSound);

	m_pThrowedEggManager->Update(elapsedSec, m_Gravity, level, level.GetEnemyShapes());

	Clamp(level);

	UpdateHitBox();

	UpdateAnimationFrames(elapsedSec);

	UpdateSprint(elapsedSec);

	UpdateJump(elapsedSec, level);

	UpdateCrossHair(elapsedSec);

	Respawn(level.GetRespawnPoint(), level, hud);

	level.HasReachedEnd(m_Shape);
}

void Avatar::Draw() const
{
	DrawAnimation();
	m_pThrowedEggManager->Draw();
}

Rectf Avatar::GetEggBox() const
{
	return m_pThrowedEggManager->GetEggBox();
}

void Avatar::HandleKeyStates(float elapsedSec, bool isOnGround)
{
	bool IsKeyPressed{ false };
	const Uint8* state = SDL_GetKeyboardState(nullptr);
	//The many checks look a bit messy but it is for fixing state bugs, where if you press multiple buttons at once it glitches
	if (!m_IsDead)
	{
		if (m_AnimationState == AnimationState::yoshiMarioJump && isOnGround && (m_Velocity.x < 0.f || m_Velocity.x > 0.f))
		{
			m_AnimationState = AnimationState::yoshiMarioIdle;
		}
		if (m_AnimationState != AnimationState::yoshiMarioDuck)
		{
			m_CanCrouchSoundBePlayed = false;
		}
		if (state[SDL_SCANCODE_W] && isOnGround)
		{
			m_FrameResetted = false;
			ResetAnimation();
			IsKeyPressed = true;
			m_Velocity.y = m_JumpSpeed;
			m_AnimationState = AnimationState::yoshiMarioJump;
			if (m_JumpSpeed > m_NormalJumpSpeed)
			{
				//m_pBounceBallJumpSound->StopAll();
				m_pBounceBallJumpSound->Play(0);
			}
			else
			{
				//m_pJumpSound->StopAll();
				m_pJumpSound->Play(0);
			}
			
		}
		if (state[SDL_SCANCODE_SPACE] && m_AnimationState == AnimationState::yoshiMarioJump)
		{
			if (m_CanClimbJump)
			{
				m_FrameResetted = false;
				ResetAnimation();
				IsKeyPressed = true;
				m_CanClimbJump = false;
				m_CanTimeSprint = false;
				m_Velocity.y = -20.f;
				m_AnimationState = AnimationState::yoshiMarioClimbJump;
				//m_FlutterJumpSound->StopAll();
				m_FlutterJumpSound->Play(0);
			}
		}
		if (state[SDL_SCANCODE_SPACE] && isOnGround && (m_AnimationState == AnimationState::yoshiMarioIdle /*|| m_AnimationState == AnimationState::yoshiMarioDuck*/))
		{
			m_FrameResetted = false;
			ResetAnimation();
			IsKeyPressed = true;
			m_AnimationState = AnimationState::yoshiMarioAiming;	
			//m_pAimingSound->Play(0);

		}
		if (state[SDL_SCANCODE_LSHIFT] && !state[SDL_SCANCODE_SPACE] && isOnGround && m_AnimationState == AnimationState::yoshiMarioAiming)
		{
			m_FrameResetted = false;
			ResetAnimation();
			IsKeyPressed = true;
			m_AnimationState = AnimationState::yoshiMarioShooting;
		}
		
		if (state[SDL_SCANCODE_A])
		{
			IsKeyPressed = true;
			m_IsFacingLeft = true;
			m_Velocity.x = -m_HorSpeed;
			m_CanTimeSprint = true;
		}
		if (state[SDL_SCANCODE_D])
		{
			IsKeyPressed = true;
			m_IsFacingLeft = false;
			m_Velocity.x = m_HorSpeed;
			m_CanTimeSprint = true;
		}
		if (state[SDL_SCANCODE_S] && isOnGround && (m_AnimationState != AnimationState::yoshiMarioSprint && m_AnimationState != AnimationState::yoshiMarioRun))
		{
			m_FrameResetted = false;
			ResetAnimation();
			if (!m_CanCrouchSoundBePlayed)
			{
				m_pCrouchSound->Play(0);
				m_CanCrouchSoundBePlayed = true;
			}
			IsKeyPressed = true;
			m_AnimationState = AnimationState::yoshiMarioDuck;
		}
		if (m_Velocity.Length() < 0.005f && m_AnimationState != AnimationState::yoshiMarioDuck && m_AnimationState != AnimationState::yoshiMarioAiming && m_AnimationState != AnimationState::yoshiMarioShooting)
		{
			ResetAnimation();
			m_AnimationState = AnimationState::yoshiMarioIdle;
		}
		if (!IsKeyPressed)
		{
			m_Velocity.x = 0.f;
			m_CanTimeSprint = false;
		}
	}
}

void Avatar::HandlePhysics(float elapsedSec)
{
	m_Shape.bottom += m_Velocity.y * elapsedSec;
	m_Shape.left += m_Velocity.x * elapsedSec;
}

void Avatar::UpdateAnimationFrames(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime > 1.f / m_NrFramesPerSec)
	{
		if (m_AnimationState == AnimationState::yoshiMarioJump)
		{
			if (m_Velocity.y > m_NormalJumpSpeed)
			{
				++m_AnimFrame %= 2;
			}
			else if (m_Velocity.y <= 0.f && m_Velocity.y >= m_LongFallSpeed)
			{
				m_AnimFrame = 2;
			}
			else if (m_Velocity.y < m_LongFallSpeed)
			{
					m_AnimFrame = 3;
			}
		}
		else
		{
			++m_AnimFrame %= m_NrOffFrames;
			m_AnimTime -= 1.f / m_NrFramesPerSec;
		}
	}
}

void Avatar::DrawAnimation() const
{
	Rectf srcRect{};
	srcRect.width = m_ClipWidth;
	srcRect.height = m_ClipHeight;
	srcRect.left = (m_AnimFrame % m_Columns) * srcRect.width;
	srcRect.bottom = (m_AnimFrame / m_Columns + 1) * srcRect.height;

	Rectf srcRectCrossHair{};
	srcRectCrossHair.width = m_pCrossHairTexture->GetWidth()/2;
	srcRectCrossHair.height = m_pCrossHairTexture->GetHeight();
	srcRectCrossHair.left = (m_AnimFrame % m_Columns) * srcRectCrossHair.width;
	srcRectCrossHair.bottom = srcRectCrossHair.height;
	
	glPushMatrix();
	{
		glTranslatef(m_Shape.left + m_Shape.width / 2.0f, m_Shape.bottom, 0.0f);
		//glTranslatef(m_CrossHairBox.left + 200.f, m_Shape.bottom, 0.0f);
		if (m_IsFacingLeft)
		{
			glScalef(-1.f, 1.f, 1.f);
		}
		Rectf dstRect{ -m_Shape.width / 2, 0.f, m_Shape.width, m_Shape.height };

		m_pAnimationStateTextures[int(m_AnimationState)]->Draw(dstRect, srcRect);

	}
	glPopMatrix();
	glPushMatrix();
	{
		glTranslatef(m_CrossHairBox.left + m_CrossHairBox.width / 2.0f, m_CrossHairBox.bottom, 0.0f);
		
		Rectf dstRect{ -m_CrossHairBox.width / 2, 0.f, m_CrossHairBox.width, m_CrossHairBox.height };

		if (m_AnimationState == AnimationState::yoshiMarioAiming)
		{
			m_pCrossHairTexture->Draw(dstRect, srcRectCrossHair);
		}
	}
	glPopMatrix();
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));
	//utils::DrawRect(m_Shape, 1.5f);
	//utils::DrawRect(m_CrossHairBox, 1.f);
}

void Avatar::Clamp(const Level& level)
{
	if (m_Shape.left <= level.GetBoundaries().left)
	{
		m_Shape.left = level.GetBoundaries().left;
	}
	else if (m_Shape.left + m_Shape.width >= level.GetBoundaries().left + level.GetBoundaries().width)
	{
		m_Shape.left = level.GetBoundaries().left + level.GetBoundaries().width - m_Shape.width;
	}
	if (m_Shape.bottom <= level.GetBoundaries().bottom)
	{
		m_Shape.bottom = level.GetBoundaries().bottom;
	}
	else if (m_Shape.bottom + m_Shape.height >= level.GetBoundaries().bottom + level.GetBoundaries().height)
	{
		m_Shape.bottom = level.GetBoundaries().bottom + level.GetBoundaries().height - m_Shape.height;
	}
}

void Avatar::ResetAnimation()
{
	
	if (!m_FrameResetted)
	{
		m_AnimFrame = 0;
		m_AnimTime = 0.f;
		m_FrameResetted = true;
	}
}

void Avatar::UpdateHitBox()
{
	switch (m_AnimationState)
	{
		case Avatar::AnimationState::yoshiMarioIdle:
			m_ClipHeight = 33.f;
			m_ClipWidth = 35.f;
			m_Shape.width = 35.f;
			m_Shape.height = 33.f;
			m_NrOffFrames = 33;
			m_Columns = 5;
			m_NrFramesPerSec = 7;
			break;
		case Avatar::AnimationState::yoshiMarioRun:
			m_ClipHeight = 33.f;
			m_ClipWidth = 37.f;
			m_Shape.width = 37.f;
			m_Shape.height = 33.f;
			m_NrOffFrames = 14;
			m_Columns = 14;
			m_NrFramesPerSec = 14;
			break;
		case Avatar::AnimationState::yoshiMarioJump:
			m_ClipHeight = 36.f;
			m_ClipWidth = 34.f;
			m_Shape.width = 34.f;
			m_Shape.height = 36.f;
			m_NrOffFrames = 5;
			m_Columns = 5;
			m_NrFramesPerSec = 2;
			break;
		case Avatar::AnimationState::yoshiMarioClimbJump:
			m_ClipHeight = 34.f;
			m_ClipWidth = 34.f;
			m_Shape.width = 34.f;
			m_Shape.height = 34.f;
			m_NrOffFrames = 4;
			m_Columns = 4;
			m_NrFramesPerSec = 22;
			break;
		//case Avatar::AnimationState::yoshiIdle:
		//	m_ClipHeight = 32.f;
		//	m_ClipWidth = 31.f;
		//	m_Shape.width = 32.f;
		//	m_Shape.height = 31.f;
		//	m_NrOffFrames = 25;
		//	m_Columns = 5;
		//	m_NrFramesPerSec = 7;
		//	break;
		case Avatar::AnimationState::yoshiMarioSprint:
			m_ClipHeight = 35.f;
			m_ClipWidth = 41.f;
			m_Shape.width = 42.f;
			m_Shape.height = 35.f;
			m_NrOffFrames = 3;
			m_Columns = 3;
			m_NrFramesPerSec = 21;
			break;
		case Avatar::AnimationState::yoshiMarioDuck:
			m_ClipHeight = 28.f;
			m_ClipWidth = 34.f;
			m_Shape.width = 34.f;
			m_Shape.height = 28.f;
			m_NrOffFrames = 6;
			m_Columns = 6;
			m_NrFramesPerSec = 2;
			break;
		case Avatar::AnimationState::yoshiMarioAiming:
			m_ClipHeight = 38.f;
			m_ClipWidth = 26.f;
			m_Shape.width = 26.f;
			m_Shape.height = 38.f;
			m_NrOffFrames = 2;
			m_Columns = 2;
			m_NrFramesPerSec = 18;
			break;
		case Avatar::AnimationState::yoshiMarioDying:
			m_ClipHeight = 43.f;
			m_ClipWidth = 30.f;
			m_Shape.width = 30.f;
			m_Shape.height = 43.f;
			m_NrOffFrames = 9;
			m_Columns = 9;
			m_NrFramesPerSec = 6;
			break;
		default:
			break;
	}
}

void Avatar::UpdateSprint(float elapsedSec)
{
	
	if (m_CanTimeSprint)
	{
		if (m_SprintAccuTime >= m_SprintTime)
		{
			m_HorSpeed = m_NormalHorSpeed * 1.75f;

			if (m_AnimationState != AnimationState::yoshiMarioJump && m_AnimationState != AnimationState::yoshiMarioClimbJump)
			{
				m_AnimationState = AnimationState::yoshiMarioSprint;
			}
		}
		else
		{
			if (m_AnimationState != AnimationState::yoshiMarioJump && m_AnimationState != AnimationState::yoshiMarioClimbJump)
			{
				m_SprintAccuTime += elapsedSec;
				m_HorSpeed = m_NormalHorSpeed + ((m_NormalHorSpeed/5) * m_SprintAccuTime);
				m_AnimationState = AnimationState::yoshiMarioRun;
			}
		}
	}
	else
	{
			m_SprintAccuTime = 0.f;
			m_HorSpeed = m_NormalHorSpeed;
	}
}

void Avatar::UpdateJump(float elapsedSec, const Level& level)
{
	if (m_AnimationState == AnimationState::yoshiMarioClimbJump && !m_CanClimbJump)
	{
		if (m_JumpClimbAccuTime >= m_JumpClimbTime)
		{
			m_AnimationState = AnimationState::yoshiMarioJump;
		}
		if (level.IsOnGround(m_Shape, m_Velocity,m_JumpSpeed, m_NormalJumpSpeed))
		{
			m_AnimationState = AnimationState::yoshiMarioIdle;
		}
		else
		{
			m_JumpClimbAccuTime += elapsedSec;
			m_Velocity.y += m_ClimbingSpeed * elapsedSec;
			m_HorSpeed = m_NormalHorSpeed * 0.8f;
		}
	}
	else
	{
		m_JumpClimbAccuTime = 0.f;
	}
}

void Avatar::UpdateCrossHair(float elapsedSec)
{
	if (m_AnimationState == AnimationState::yoshiMarioShooting)
	{
		Point2f beginPos{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };

		Point2f endPos{};
		endPos.x = m_CrossHairBox.left + m_CrossHairBox.width / 2;
		endPos.y = m_CrossHairBox.bottom + m_CrossHairBox.height / 2;

		Vector2f velocity{};
		velocity.x = endPos.x - beginPos.x;
		velocity.y = endPos.y - beginPos.y;

		m_pThrowedEggManager->AddItem(beginPos, velocity);
		m_pEggShotSound->Play(0);
		m_AnimationState = AnimationState::yoshiMarioIdle;
	}
	if(m_AnimationState == AnimationState::yoshiMarioAiming)
	{
		m_CrossHairAngle += m_CrossHairAngleSpeed * elapsedSec;

		if (m_CrossHairAngle >= m_MaxAngle && m_CrossHairAngleSpeed > 0.f)
		{
			m_CrossHairAngleSpeed *= -1;
		}

		if (m_CrossHairAngle <= m_MinAngle && m_CrossHairAngleSpeed < 0.f)
		{
			m_CrossHairAngleSpeed *= -1;
		}

		m_CrossHairBox.bottom = ReturnCrossHairLocations().y;
		m_CrossHairBox.left = ReturnCrossHairLocations().x;
	}
}

Point2f Avatar::ReturnCrossHairLocations()
{
	Point2f pos{};
	Point2f displacement{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2 };

	if (m_IsFacingLeft)
	{
		pos.x = m_CrossHairOffSet * -cos(m_CrossHairAngle * float(M_PI) / 180.f) + displacement.x;
		pos.y = m_CrossHairOffSet * sin(m_CrossHairAngle * float(M_PI) / 180.f) + displacement.y;
	}
	else
	{
		pos.x = m_CrossHairOffSet * cos(m_CrossHairAngle * float(M_PI) / 180.f) + displacement.x;
		pos.y = m_CrossHairOffSet * sin(m_CrossHairAngle * float(M_PI) / 180.f) + displacement.y;
	}

	pos.x -= m_CrossHairBox.width / 2;
	pos.y -= m_CrossHairBox.height / 2;

	return pos;
}

void Avatar::Respawn(const Point2f& respawnPoint, Level& level, const HUD& hud)
{

	if (m_IsDead)
	{	
		
		m_Velocity.x = 0.f;
		m_AnimationState = AnimationState::yoshiMarioDying;
		ResetAnimation();

		if (!m_IsHitSoundPlayed)
		{
			m_pYoshiHitSound->Play(0);
			m_IsHitSoundPlayed = true;
		}
		
		if (m_AnimFrame == 8)
		{
			m_Shape.bottom = respawnPoint.y;
			m_Shape.left = respawnPoint.x;
			m_Velocity.x = 0.f;
			m_Velocity.y = 0.f;
			m_AnimationState = AnimationState::yoshiMarioIdle;
			m_IsDead = false;		
			m_FrameResetted = false;
			m_IsHitSoundPlayed = false;
			level.RefreshLevel();
		}
	}
	if (m_CanGameBeResetted && hud.GetGameState() == HUD::GameState::transitionToLevel)
	{
		m_Shape.bottom = respawnPoint.y;
		m_Shape.left = respawnPoint.x;
		m_Velocity.x = 0.f;
		m_Velocity.y = 0.f;
		m_CanSprint = false;
		m_CanTimeSprint = false;
		m_AnimationState = AnimationState::yoshiMarioIdle;
		m_IsDead = false;
		m_FrameResetted = false;
		m_IsHitSoundPlayed = false;
		m_IsFacingLeft = false;
		level.RefreshLevel();
	}
	if (!m_CanGameBeResetted && hud.GetGameState() == HUD::GameState::playingLevel)
	{
		m_CanGameBeResetted = true;
	}
	
}

