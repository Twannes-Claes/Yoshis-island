#pragma once
class Texture;
class TextureManager;
class SoundStream;
class SoundEffect;
class SoundManager;

class HUD final
{
	public:
		

		enum class GameState
		{
			mainScreen = 0,
			mainScreenTransition,
			transitionToLevel,
			playingLevel,
			playingLevelTransition,
			transitionToMainScreen,
			settingScreen
		};

		explicit HUD(const Rectf& boundaries, TextureManager* textureManager, SoundManager* soundManager, const int volume);

		HUD(const HUD& other) = delete;
		HUD& operator=(const HUD& other) = delete;
		HUD(HUD&& other) = delete;
		HUD& operator=(HUD&& other) = delete;

		~HUD();

		void Draw() const;

		void DrawSettingScreen(const int volume, const int maxVolume);

		void UpdateFrames(float elapsedSec, bool isLevelCleared);

		void BeginPlay();

		GameState GetGameState() const;

		void RegulateMusicSound(const int volume);

		void ToggleSettingsScreen();

		void SetCanGoBack();

	private:

		Rectf m_Boundaries;

		const Texture* m_pYoshiIslandTexture;
		const Texture* m_pTitleScreen;
		const Texture* m_pTransitionScreen;
		const Texture* m_pSettingScreenTexture;

		const Texture* m_pSettingsWord;
		const Texture* m_pSettingsVolumeIcon;
		const Texture* m_pMasterVolumeWord;
		const Texture* m_pMutedWord;
		const Texture* m_pYesWord;
		const Texture* m_pNoWord;
		const Texture* m_pMuteExplanation;
		const Texture* m_pLeaveWord;
		const Texture* m_pPressToPlay;

		const SoundStream* m_pMainMenuSong;

		const SoundEffect* m_pNintendoSound;
		const SoundEffect* m_pPauseSound;
		const SoundEffect* m_pLeaveSettingsSound;

		bool m_IsNintendoPlayed{ false };
		bool m_IsInSettings{ false };
		bool m_CanGoBack{ false };

		GameState m_GameState;
		GameState m_TempGameState{};

		const int m_NrOffFramesIsland{ 512 };
		const int m_NrFramesPerSecIsland{ 32 };
		float m_AnimTimeIsland{};
		int m_AnimFrameIsland{};
		const int m_ColumnsIsland{ 32 };
		const int m_RowsIsland{ 16 };

		const int m_NrOffFramesTitle{ 24 };
		int m_NrFramesPerSecTitle{ 10 };
		float m_AnimTimeTitle{};
		int m_AnimFrameTitle{};
		const int m_ColumnsTitle{ 3 };
		const int m_RowsTitle{ 9 };

		const int m_NrOffFramesEnter{ 12 };
		int m_NrFramesPerSecEnter{ 4 };
		float m_AnimTimeEnter{};
		int m_AnimFrameEnter{};
		const int m_ColumnsEnter{ 3 };
		const int m_RowsEnter{ 4 };

		const int m_NrOffFramesTransition{ 37};
		int m_NrFramesPerSecTransition{ 9 };
		float m_AnimTimeTransition{};
		int m_AnimFrameTransition{};
		const int m_ColumnsTransition{ 3 };
		const int m_RowsTransition{ 13 };

		void DrawIslandAndTitle() const;

		void DrawTransition() const;
};

