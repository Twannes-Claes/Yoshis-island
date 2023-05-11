#include "pch.h"
#include "Camera.h"
#include "utils.h"

Camera::Camera(const float width, const float height)
	:m_Width{width}
	,m_Height{height}
	,m_LevelBoundaries{Rectf(0.f, 0.f, width, height)}
{
	m_Zoom = m_OriginalZoom;
}

void Camera::Update(float elapsedSec, bool canZoom)
{
	if (canZoom)
	{
		float zoomPerSecond{ (m_MaxZoom - m_OriginalZoom) / m_ZoomTime };
		if (m_ZoomTimer >= m_ZoomTime)
		{
			return;
		}
		m_ZoomTimer += elapsedSec;
		m_Zoom += zoomPerSecond * elapsedSec;
	}
	else
	{
		m_Zoom = m_OriginalZoom;
		m_ZoomTimer = 0.f;
	}
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transforms(const Rectf& target)
{
	Point2f position{ Track(target) };

	glScalef(m_Zoom, m_Zoom, 1.f);
	glTranslatef(-position.x, -position.y, 0);

}

void Camera::TransformBackground(const Rectf& target)
{
	Point2f position{ Track(target) };

	glScalef(m_Zoom, m_Zoom, 1.f);
	glTranslatef(-position.x/1.5f, -position.y, 0);
}

Point2f Camera::Track(const Rectf& target)
{
	Point2f bottomLeftPos;
	bottomLeftPos.x = (target.left + target.width * 0.5f) * m_Zoom - m_Width * 0.5f;
	bottomLeftPos.y = (target.bottom + target.height * 0.5f) * m_Zoom - m_Height * 0.4f;

	bottomLeftPos.x /= m_Zoom;
	bottomLeftPos.y /= m_Zoom;

	Clamp(bottomLeftPos);
	return bottomLeftPos;
}

void Camera::Clamp(Point2f& bottomLeftPos)
{
	if (bottomLeftPos.x <= m_LevelBoundaries.left)
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	if (bottomLeftPos.y <= m_LevelBoundaries.bottom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	if (bottomLeftPos.x > m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width / m_Zoom)
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width / m_Zoom;
	}
	if (bottomLeftPos.y > m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height / m_Zoom)
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height / m_Zoom;
	}
}
