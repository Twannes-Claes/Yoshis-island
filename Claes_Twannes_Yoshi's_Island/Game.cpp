#include "pch.h"
#include <iostream>

#include "Game.h"
#include "utils.h"
#include "Level.h"
#include "Avatar.h"
#include "Camera.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "HUD.h"

Game::Game( const Window& window )
	:m_Window{ window }
	,m_pSoundManager{ new SoundManager{} }
	,m_pTextureManager{ new TextureManager{} }
	,m_MaxVolume{50}
{	 
	Initialize( );
}

Game::~Game( )
{
	Cleanup();
}

void Game::Initialize( )
{
	m_Volume = m_MaxVolume / 2;
	//ShowTestMessage( );
	m_pLevel = new Level{m_pTextureManager, m_pSoundManager, m_Volume};
	m_pCamera = new Camera{ m_Window.width, m_Window.height };
	m_pCamera->SetLevelBoundaries(m_pLevel->GetBoundaries());
	m_pAvatar = new Avatar(m_pLevel->GetRespawnPoint(), m_pTextureManager, m_pSoundManager, m_Volume);
	m_pHUD = new HUD(Rectf(0.f, 0.f, m_Window.width, m_Window.height), m_pTextureManager, m_pSoundManager, m_Volume);
}

void Game::Cleanup()
{
	delete m_pLevel;
	delete m_pAvatar;
	delete m_pCamera;
	delete m_pTextureManager;
	delete m_pSoundManager;
	delete m_pHUD;
}

void Game::Update( float elapsedSec )
{


		if (m_pHUD->GetGameState() == HUD::GameState::playingLevel || m_pHUD->GetGameState() == HUD::GameState::transitionToLevel)
		{
			m_pAvatar->Update(elapsedSec, *m_pLevel, *m_pHUD);

			m_pLevel->Update(elapsedSec);

			m_pCamera->Update(elapsedSec,m_pLevel->CanZoom());
		}

		m_pHUD->UpdateFrames(elapsedSec, m_pLevel->HasReachedEnd(m_pAvatar->GetShape()));
		
	std::cout << "FPS: " << 1 / elapsedSec << '\n';

}

void Game::Draw() const
{
	ClearBackground();

	if ((int(m_pHUD->GetGameState()) >= 2 && int(m_pHUD->GetGameState()) <= 4) || m_pHUD->GetGameState() == HUD::GameState::settingScreen)
	{
		glPushMatrix();
		{
			m_pCamera->TransformBackground(m_pAvatar->GetShape());
			m_pLevel->DrawFarBack();
		}
		glPopMatrix();

		glPushMatrix();
		{
			m_pCamera->Transforms(m_pAvatar->GetShape());

			m_pLevel->Draw();

			m_pAvatar->Draw();
			//m_pLevel->DrawRaycast();
		}
		glPopMatrix();
	}
	
	
	m_pHUD->Draw();
	int tempVolume{};
	if (!m_IsMuted)
	{
		tempVolume = m_Volume;
	}
	m_pHUD->DrawSettingScreen(tempVolume, m_MaxVolume);
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
		case SDLK_i:
			ShowTestMessage();
			break;
		case SDLK_m:
			m_IsMuted = !m_IsMuted;
			if (m_IsMuted)
			{
				m_pSoundManager->RegulateVolume(0);
				m_pLevel->RegulateMusicSound(0);
				m_pHUD->RegulateMusicSound(0);
			}
			else
			{
				m_pSoundManager->RegulateVolume(m_Volume);
				m_pLevel->RegulateMusicSound(m_Volume);
				m_pHUD->RegulateMusicSound(m_Volume);
			}
			break;
		case SDLK_UP:
			if (m_Volume < m_MaxVolume)
			{
				m_Volume += 5;
			}
			m_pSoundManager->RegulateVolume(m_Volume);
			m_pLevel->RegulateMusicSound(m_Volume);
			m_pHUD->RegulateMusicSound(m_Volume);
			break;
		case SDLK_DOWN:
			if (m_Volume > 0)
			{
				m_Volume -= 5;
			}
			m_pSoundManager->RegulateVolume(m_Volume);
			m_pLevel->RegulateMusicSound(m_Volume);
			m_pHUD->RegulateMusicSound(m_Volume);
			break;
		case SDLK_RETURN:
			m_pHUD->BeginPlay();
			m_pHUD->SetCanGoBack();
			break;
		case SDLK_ESCAPE:
			m_pHUD->ToggleSettingsScreen();
		default:
			break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::ShowTestMessage( ) const
{
	std::cout << "Movement controls\n" << '\n';

	std::cout << "Press ENTER to begin\n";
	std::cout << "Use WASD to move around\n";
	std::cout << "When moving sideways for a second and a half u start SPRINTING\n";
	std::cout << "Press W to jump when on ground\n";
	std::cout << "When in jump press SPACE to flutter jump\n";
	std::cout << "When on ground press SPACE to aim an egg\n";
	std::cout << "When aiming egg press LSHIFT to throw egg at chosen location\n";
	std::cout << "Press S to duck\n";
	std::cout << "Press M to mute all sound, and you can also use UP and DOWN to regulate the sound\n";
	std::cout << "Press ESC to open Settings menu\n";
	std::cout << "While in settings use the same keys for sound control\n";
	std::cout << "While in settings and in game press ENTER to return to the main menu\n";
}

