#pragma once
class Level;
class Avatar;
class Camera;
class HUD;
class SoundManager;
class TextureManager;

class Game final
{
public:

	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );
	void ClearBackground() const;
private:
	// DATA MEMBERS
	const Window m_Window;

	Level* m_pLevel;
	Avatar* m_pAvatar;
	Camera* m_pCamera;
	SoundManager* m_pSoundManager;
	TextureManager* m_pTextureManager;
	HUD* m_pHUD;
	
	int m_Volume;
	const int m_MaxVolume;
	bool m_IsMuted{false};

	// FUNCTIONS
	void Initialize( );
	void Cleanup();
	void ShowTestMessage( ) const;
};