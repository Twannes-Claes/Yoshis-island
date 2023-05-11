#pragma once
class Camera final
{
	public:

		explicit Camera(const float width, const float height);
		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;
		Camera(Camera&&) = delete;
		Camera& operator=(Camera&&) = delete;

		void Update(float elapsedSec, bool canZoom);

		void SetLevelBoundaries(const Rectf& levelBoundaries);
		void Transforms(const Rectf& target);

		void TransformBackground(const Rectf& target);

	private:

		const float m_Width;
		const float m_Height;
		Rectf m_LevelBoundaries;
		float m_Zoom;
		const float m_OriginalZoom{2.2f};
		const float m_MaxZoom{ 6.9f };

		float m_ZoomTimer{};
		const float m_ZoomTime{ 8.f };
		
		Point2f Track(const Rectf& target);
		void Clamp(Point2f& bottomLeftPos);
};

