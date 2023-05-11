#pragma once
#include "Vector2f.h"
#include "GameObject.h"

class Texture;
class Level;

class ThrowedEgg final: public GameObject
{
	public:

		explicit ThrowedEgg(const Point2f& pos, const Vector2f& velocity, const Texture* texture);
		~ThrowedEgg() = default;

		ThrowedEgg(const ThrowedEgg&) = delete;
		ThrowedEgg& operator=(const ThrowedEgg&) = delete;
		ThrowedEgg(ThrowedEgg&&) = delete;
		ThrowedEgg& operator=(ThrowedEgg&&) = delete;

		void Draw() const;
		void Update(float elapsedSec, float gravity);
		bool IsOutsideBoundaries(const Level& level) const;

	private:

		Vector2f m_Velocity;
		const Texture* m_pTexture;
};

