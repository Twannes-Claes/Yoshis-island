#pragma once
class Texture;

class GameObject
{
	public:

		GameObject(const Rectf& shape);
		virtual ~GameObject() = default;

		virtual void Draw() const;
		virtual void Update();

		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		Rectf GetShape() const;

	protected:
		
		Rectf m_Shape;
		
	private:
};

