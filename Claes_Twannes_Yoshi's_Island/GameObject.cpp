#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(const Rectf& shape)
	:m_Shape{shape}
{
}

void GameObject::Draw() const
{
}

void GameObject::Update()
{
}

Rectf GameObject::GetShape() const
{
	return m_Shape;
}
