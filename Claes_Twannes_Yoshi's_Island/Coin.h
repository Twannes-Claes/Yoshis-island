#pragma once
#include "GameObject.h"

class Texture;

class Coin final: public GameObject
{
public:

	explicit Coin(const Point2f& center, const Texture* textureCoin, const Texture* textureEffect);
	~Coin() = default;

	Coin(const Coin&) = delete;
	Coin& operator=(const Coin&) = delete;
	Coin(Coin&&) = delete;
	Coin& operator=(Coin&&) = delete;

	void Update(float elapsedSec);
	void Draw() const;

	void SetIsPickedUp();

	bool GetIsPickedUp() const;

	bool IsAbleToDelete() const;
	bool IsOverlapping(const Rectf& actorShape, const Rectf& projectileShape) const;

private:

	const Texture* m_pTextureCoin;
	const Texture* m_pTextureCoinEffect;

	float m_ClipHeight{ 16.f };
	float m_ClipWidth{ 12.f };

	int m_NrOffFrames{ 4 };
	int m_NrFramesPerSec{ 3 };
	float m_AnimTime{};
	int m_AnimFrame{};
	int m_Columns{4};
	float m_EffectTimer{};

	bool m_IsPickedUp{ false };
	bool m_IsAbleToDelete{ false };
};


