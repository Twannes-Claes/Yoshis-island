#pragma once
#include "ShyGuy.h"
#include "Texture.h"
#include <vector>
#include "SoundEffect.h"

class TextureManager;
class SoundManager;

class ShyGuyManager final
{
public:
	ShyGuyManager(TextureManager* textureManager, SoundManager* soundManager, const int volume);
	~ShyGuyManager();

	ShyGuyManager(const ShyGuyManager&) = delete;
	ShyGuyManager& operator=(const ShyGuyManager&) = delete;
	ShyGuyManager(ShyGuyManager&&) = delete;
	ShyGuyManager& operator=(ShyGuyManager&&) = delete;

	void Draw() const;
	void Update(float elapsedSec, const float gravity, const std::vector<std::vector<Point2f>>& vertices);

	ShyGuy* AddItem(const Point2f& bottomLeft, const bool isFacingLeft, const float walkDistance);

	size_t Size() const;

	void HandleExternalCollision(const Rectf& actorShape, const Rectf& projectileShape, const Vector2f& actorVelocity, const Rectf& boundaries, bool& isDead);

	std::vector<Rectf> GetShapes() const;

	void RefreshShyGuys();

private:

	std::vector<ShyGuy*> m_pItems{};

	const Texture* m_pTexture;

	const SoundEffect* m_pDeathSound;
};

