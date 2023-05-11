#include "pch.h"
#include "HUD.h"
#include "TextureManager.h"
#include "Texture.h"
#include "SoundStream.h"
#include "SoundManager.h"
#include "SoundEffect.h"

HUD::HUD(const Rectf& boundaries, TextureManager* textureManager, SoundManager* soundManager, const int volume)
	:m_Boundaries{boundaries}
	,m_pYoshiIslandTexture{ textureManager->GetTexturePointer("Resources/Images/HUD/IslandSheet.png") }
	,m_pTitleScreen{ textureManager->GetTexturePointer("Resources/Images/HUD/Title.png") }
	,m_pTransitionScreen{textureManager->GetTexturePointer("Resources/Images/HUD/Transition.png")}
	,m_pMainMenuSong{new SoundStream("Resources/Sounds/MainScreenSong.mp3")}
	,m_pNintendoSound{soundManager->GetSoundPointer("Resources/Sounds/Nintendo.wav",volume)}
	,m_pSettingScreenTexture{textureManager->GetTexturePointer("Resources/Images/HUD/SettingScreen.png")}
	,m_pPauseSound{ soundManager->GetSoundPointer("Resources/Sounds/Pause.wav", volume) }
	,m_pLeaveSettingsSound{ soundManager->GetSoundPointer("Resources/Sounds/LeaveSettings.wav", volume) }
	,m_pSettingsWord{textureManager->GetTexturePointerByFont("-Settings-","Resources/Font/YoshiFont.ttf", 200, Color4f(0.f, 0.f,0.f,1.f))}
	,m_pSettingsVolumeIcon{textureManager->GetTexturePointerByFont("-","Resources/Font/YoshiFont.ttf", 200, Color4f(0.f, 0.f,0.f,1.f))}
	,m_pMasterVolumeWord{textureManager->GetTexturePointerByFont("Master volume >","Resources/Font/YoshiFont.ttf", 200, Color4f(0.f, 0.f,0.f,1.f))}
	,m_pMutedWord{textureManager->GetTexturePointerByFont("Muted?","Resources/Font/YoshiFont.ttf", 200, Color4f(0.f, 0.f,0.f,1.f))}
	,m_pYesWord{textureManager->GetTexturePointerByFont("Yes","Resources/Font/YoshiFont.ttf", 200, Color4f(0.2f, .8f,0.2f,1.f))}
	,m_pNoWord{textureManager->GetTexturePointerByFont("No","Resources/Font/YoshiFont.ttf", 200, Color4f(0.8f, 0.2f,0.2f,1.f))}
	,m_pMuteExplanation{textureManager->GetTexturePointerByFont("Press 'M' to mute-unmute","Resources/Font/YoshiFont.ttf", 200, Color4f(0.f, 0.f,0.f,1.f))}
	,m_pLeaveWord{textureManager->GetTexturePointerByFont("ENTER to leave","Resources/Font/YoshiFont.ttf", 200, Color4f(0.411f, 0.027f, 0.027f,1.f))}
	,m_pPressToPlay{textureManager->GetTexturePointer("Resources/Images/HUD/PressEnter.png")}
{
	m_GameState = GameState::mainScreen;
	m_pMainMenuSong->SetVolume(volume);
	m_pMainMenuSong->Play(true);
}

HUD::~HUD()
{
	delete m_pMainMenuSong;
}

void HUD::Draw() const
{
	DrawIslandAndTitle();
	DrawTransition();
	
}

void HUD::UpdateFrames(float elapsedSec, bool isLevelCleared)
{
	//update variables
	if (m_GameState == GameState::mainScreen || m_GameState == GameState::mainScreenTransition || m_GameState == GameState::transitionToMainScreen)
	{
		m_AnimTimeIsland += elapsedSec;
		if (m_AnimTimeIsland > 1.f / m_NrFramesPerSecIsland)
		{
			++m_AnimFrameIsland %= m_NrOffFramesIsland;
			m_AnimTimeIsland -= 1.f / m_NrFramesPerSecIsland;
		}

		m_AnimTimeTitle += elapsedSec;
		if (m_AnimTimeTitle > 1.f / m_NrFramesPerSecTitle)
		{
			++m_AnimFrameTitle %= m_NrOffFramesTitle;
			m_AnimTimeTitle -= 1.f / m_NrFramesPerSecTitle;
		}

		m_AnimTimeEnter += elapsedSec;
		if (m_AnimTimeEnter > 1.f / m_NrFramesPerSecEnter)
		{
			++m_AnimFrameEnter %= m_NrOffFramesEnter;
			m_AnimTimeEnter -= 1.f / m_NrFramesPerSecEnter;
		}
	}
	if (m_GameState == GameState::mainScreenTransition || m_GameState == GameState::playingLevelTransition || m_GameState == GameState::transitionToLevel || m_GameState == GameState::transitionToMainScreen)
	{
		m_AnimTimeTransition += elapsedSec;
		if (m_AnimTimeTransition > 1.f / m_NrFramesPerSecTransition)
		{

			if (m_AnimFrameTransition > m_NrOffFramesTransition / 2)
			{
				if (m_GameState == GameState::mainScreenTransition)
				{
					m_pMainMenuSong->Stop();
					m_GameState = GameState::transitionToLevel;
				}
				else if (m_GameState == GameState::playingLevelTransition)
				{
					m_GameState = GameState::transitionToMainScreen;
					m_pMainMenuSong->Play(0);
				}
			}
			
			if (m_AnimFrameTransition == m_NrOffFramesTransition - 1)
			{
				if (m_GameState == GameState::transitionToLevel)
				{
					m_GameState = GameState::playingLevel;
				}
				else if (m_GameState == GameState::transitionToMainScreen)
				{
					m_GameState = GameState::mainScreen;
				}
				m_AnimFrameTransition = 0;
				m_AnimTimeTransition = 0;
			}
			else
			{
				++m_AnimFrameTransition %= m_NrOffFramesTransition;
				m_AnimTimeTransition -= 1.f / m_NrFramesPerSecTransition;
			}
		}
	}
	if (isLevelCleared && m_GameState == GameState::playingLevel)
	{
		m_GameState = GameState::playingLevelTransition;
	}
	if (isLevelCleared && m_GameState == GameState::transitionToMainScreen)
	{
		if (!m_pMainMenuSong->IsPlaying())
		{
			m_pMainMenuSong->Play(true);
		}
	}
	if (m_GameState == GameState::mainScreenTransition)
	{
		if (!m_IsNintendoPlayed)
		{
			m_pNintendoSound->Play(0);
			m_IsNintendoPlayed = true;
		}
	}
	else 
	{
		m_IsNintendoPlayed = false;
	}
}

void HUD::BeginPlay()
{
	if (m_GameState == GameState::mainScreen)
	{
		m_GameState = GameState::mainScreenTransition;
	}
}

HUD::GameState HUD::GetGameState() const
{
	return m_GameState;
}

void HUD::RegulateMusicSound(const int volume)
{
	m_pMainMenuSong->SetVolume(volume);
}

void HUD::ToggleSettingsScreen()
{
	if (m_GameState != GameState::mainScreenTransition && m_GameState != GameState::playingLevelTransition && m_GameState != GameState::transitionToLevel && m_GameState != GameState::transitionToMainScreen)
	{
		
		if (m_GameState != GameState::settingScreen)
		{
			if (!m_IsInSettings)
			{
				m_TempGameState = m_GameState;
				m_GameState = GameState::settingScreen;
				m_IsInSettings = true;
				m_pPauseSound->Play(0);
			}
			
		}
		else if (m_GameState == GameState::settingScreen && m_IsInSettings)
		{
			m_GameState = m_TempGameState;
			m_IsInSettings = false;
			m_pLeaveSettingsSound->Play(0);
		}	
	}
	
}

void HUD::SetCanGoBack()
{
	if (m_GameState == GameState::settingScreen && m_TempGameState == GameState::playingLevel)
	{
		m_CanGoBack = true;
	}
}

void HUD::DrawIslandAndTitle() const
{
	if (m_GameState != GameState::playingLevel)
	{
		if (m_GameState == GameState::mainScreen || m_GameState == GameState::mainScreenTransition || m_GameState == GameState::transitionToMainScreen || (m_GameState == GameState::settingScreen && m_TempGameState == GameState::mainScreen))
		{

			const Rectf srcRectIsland
			{
				(m_AnimFrameIsland % m_ColumnsIsland) * (m_pYoshiIslandTexture->GetWidth() / m_ColumnsIsland),
				(m_AnimFrameIsland / m_ColumnsIsland + 1) * (m_pYoshiIslandTexture->GetHeight() / m_RowsIsland),
				m_pYoshiIslandTexture->GetWidth() / m_ColumnsIsland,
				m_pYoshiIslandTexture->GetHeight() / m_RowsIsland,
			};

			const Rectf srcRectTitle
			{
				(m_AnimFrameTitle % m_ColumnsTitle) * (m_pTitleScreen->GetWidth() / m_ColumnsTitle),
				(m_AnimFrameTitle / m_ColumnsTitle + 1) * (m_pTitleScreen->GetHeight() / m_RowsTitle),
				m_pTitleScreen->GetWidth() / m_ColumnsTitle,
				m_pTitleScreen->GetHeight() / m_RowsTitle,
			};

			const Rectf dstRectTitle
			{
				m_Boundaries.width * 0.1f,
				m_Boundaries.height * 0.6f,
				m_Boundaries.width * 0.8f,
				m_Boundaries.height * 0.4f
			};

			const Rectf dstRectPress
			{
				m_Boundaries.width * 0.2f,
				m_Boundaries.height * 0.05f,
				m_Boundaries.width * 0.6f,
				m_Boundaries.height * 0.14f
			};

			const Rectf srcRectPress
			{
				(m_AnimFrameEnter % m_ColumnsEnter) * (m_pPressToPlay->GetWidth() / m_ColumnsEnter),
				(m_AnimFrameEnter / m_ColumnsEnter + 1) * (m_pPressToPlay->GetHeight() / m_RowsEnter),
				m_pPressToPlay->GetWidth() / m_ColumnsEnter,
				m_pPressToPlay->GetHeight() / m_RowsEnter,
			};

			m_pYoshiIslandTexture->Draw(m_Boundaries, srcRectIsland);

			m_pTitleScreen->Draw(dstRectTitle, srcRectTitle);

			
			/*utils::SetColor(Color4f(0.1f, 0.5f, 0.1f, 0.6f));
			utils::FillRect(dstRectPress);*/
			
			//m_pPressToPlay.

			m_pPressToPlay->Draw(dstRectPress,srcRectPress );
		}
	}
}

void HUD::DrawTransition() const
{
	if (m_GameState == GameState::mainScreenTransition || m_GameState == GameState::playingLevelTransition || m_GameState == GameState::transitionToLevel || m_GameState == GameState::transitionToMainScreen)
	{
		const Rectf srcRectTransition
		{
			(m_AnimFrameTransition % m_ColumnsTransition) * (m_pTransitionScreen->GetWidth() / m_ColumnsTransition),
			(m_AnimFrameTransition / m_ColumnsTransition + 1) * (m_pTransitionScreen->GetHeight() / m_RowsTransition),
			m_pTransitionScreen->GetWidth() / m_ColumnsTransition,
			m_pTransitionScreen->GetHeight() / m_RowsTransition,
		};

		m_pTransitionScreen->Draw(m_Boundaries, srcRectTransition);
	}
}

void HUD::DrawSettingScreen(const int volume, const int maxVolume)
{
	if (m_GameState == GameState::settingScreen)
	{
		const Rectf dstRectSettingScreen
		{
			m_Boundaries.width * 0.05f,
			m_Boundaries.height * 0.05f,
			m_Boundaries.width * 0.9f,
			m_Boundaries.height * 0.9f
		};

		const Rectf dstRectSettingsWord
		{
			m_Boundaries.width * 0.25f,
			m_Boundaries.height * 0.7f,
			m_Boundaries.width * 0.5f,
			m_Boundaries.height * 0.12f
		};

		const Rectf dstRectMasterVolume
		{
			m_Boundaries.width * 0.11f,
			m_Boundaries.height * 0.5f,
			m_Boundaries.width * 0.29f,
			m_Boundaries.height * 0.1f
		};

		const Rectf dstRectMutedWord
		{
			m_Boundaries.width * 0.11f,
			m_Boundaries.height * 0.3f,
			m_Boundaries.width * 0.2f,
			m_Boundaries.height * 0.1f
		};

		const Rectf dstRectMutedWordAnswer
		{
			m_Boundaries.width * 0.34f,
			m_Boundaries.height * 0.3f,
			m_Boundaries.width * 0.1f,
			m_Boundaries.height * 0.1f
		};

		const Rectf dstRectMuteExplanation
		{
			m_Boundaries.width * 0.15f,
			m_Boundaries.height * 0.13f,
			m_Boundaries.width * 0.7f,
			m_Boundaries.height * 0.12f
		};

		m_pSettingScreenTexture->Draw(dstRectSettingScreen);

		m_pSettingsWord->Draw(dstRectSettingsWord);

		m_pMasterVolumeWord->Draw(dstRectMasterVolume);

		m_pMutedWord->Draw(dstRectMutedWord);

		m_pMuteExplanation->Draw(dstRectMuteExplanation);

		if (volume > 0)
		{
			m_pNoWord->Draw(dstRectMutedWordAnswer);
		}
		else if(volume == 0)
		{
			m_pYesWord->Draw(dstRectMutedWordAnswer);
		}

		for (int i{}; i < volume / (maxVolume / 10); ++i)
		{
			const Rectf dstRectSoundBars
			{
				(m_Boundaries.width * 0.4f) + (m_Boundaries.width * 0.05f * i),
				m_Boundaries.height * 0.5f,
				m_Boundaries.width * 0.05f,
				m_Boundaries.height * 0.1f
			};

			m_pSettingsVolumeIcon->Draw(dstRectSoundBars);
		}
		if (m_TempGameState == GameState::playingLevel)
		{
			const Rectf dstRectLeaveWord
			{
				m_Boundaries.width * 0.51f,
				m_Boundaries.height * 0.3f,
				m_Boundaries.width * 0.33f,
				m_Boundaries.height * 0.12f
			};

			m_pLeaveWord->Draw(dstRectLeaveWord);

			if (m_CanGoBack)
			{
				m_GameState = GameState::playingLevelTransition;
				m_CanGoBack = false;
				m_IsInSettings = false;
				m_pLeaveSettingsSound->Play(0);
			}
		}
	}
}
