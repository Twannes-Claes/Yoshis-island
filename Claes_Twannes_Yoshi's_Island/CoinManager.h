#pragma once
#include "Coin.h"
#include <vector>
#include "structs.h"
#include "SoundEffect.h"

class TextureManager;
class SoundManager;

class CoinManager final
{
public:

	CoinManager( TextureManager* textureManager, SoundManager* soundManager, const int volume);
	~CoinManager( );

	CoinManager(const CoinManager&) = delete;
	CoinManager& operator=(const CoinManager&) = delete;
	CoinManager(CoinManager&&) = delete;
	CoinManager& operator=(CoinManager&&) = delete;

	void Update(const Rectf& actorShape, const Rectf& projectileShape, float elapsedSec );
	void Draw() const;

	Coin* AddItem(const Point2f& botLeft);

	size_t Size() const;

	void RefreshCoins();
	
private:

	std::vector<Coin*> m_pItems{};

	const SoundEffect* m_Effect;

	float m_EffectTimer{};

	const Texture* m_pTextureCoin;
	const Texture* m_pTextureEffect;

};
