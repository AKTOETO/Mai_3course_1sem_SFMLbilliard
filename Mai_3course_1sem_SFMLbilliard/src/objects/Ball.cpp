#include "Ball.h"

Engine::Ball::Ball(int id, float radius)
	:OneSpriteDynamicObject(id), m_radius(radius)
{}

void Engine::Ball::SetRadius(float radius)
{
	m_radius = radius;
}

float Engine::Ball::GetRadius() const
{
	return m_radius;
}
