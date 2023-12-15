#include "DynamicObject.h"

Engine::DynamicObject::DynamicObject(int id, Vector2f pos, float mass)
	: m_old_position({ 0.f,0.f }), m_id(id), m_position(pos), m_mass(mass)
{}

void Engine::DynamicObject::Update(float dT)
{
	// Verlet integration
	// xn+1 = xn + (xn - xn-1) + a * dT

	// скорость
	Vector2f vel = m_position - m_old_position;

	// сохраняем текущую позицию в старую позицию
	m_old_position = m_position;

	// вычисление ускорения
	Vector2f acc = m_force / m_mass;

	// вычисление позиции с помощью метода Верлета
	// https://en.wikipedia.org/wiki/Verlet_integration
	m_position = vel + acc * dT * dT;
}

void Engine::DynamicObject::ApplyForce(const Vector2f& force)
{
	m_force += force;
}

void Engine::DynamicObject::Move(Vector2f dist)
{
	m_position += dist;
}

void Engine::DynamicObject::SetMass(float mass)
{
	m_mass = mass;
}

void Engine::DynamicObject::SetPos(Vector2f pos)
{
	m_old_position = m_position;
	m_position = pos;
}

void Engine::DynamicObject::SetOldPos(Vector2f pos)
{
	m_old_position = pos;
}

float Engine::DynamicObject::GetMass() const
{
	return m_mass;
}

Vector2f Engine::DynamicObject::GetPos() const
{
	return m_position;
}

Vector2f Engine::DynamicObject::GetOldPos() const
{
	return m_old_position;
}

Vector2f Engine::DynamicObject::GetForce() const
{
	return m_force;
}
