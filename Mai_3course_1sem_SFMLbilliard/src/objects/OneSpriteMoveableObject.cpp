#include "OneSpriteMoveableObject.h"

Engine::OneSpriteDynamicObject::OneSpriteDynamicObject(
	int id, std::shared_ptr<CustomSprite> sprite)
	: DynamicObject(id, { 100,100 }), m_sprite(sprite)
{}

void Engine::OneSpriteDynamicObject::SetSprite(std::shared_ptr<CustomSprite> sprite)
{
	m_sprite = sprite;
}

void Engine::OneSpriteDynamicObject::SetPos(Vector2f pos)
{
	DynamicObject::SetPos(pos);
	if (m_sprite && m_sprite->m_sprite) m_sprite->m_sprite->setPosition(pos);
}

void Engine::OneSpriteDynamicObject::Move(Vector2f dist)
{
	DynamicObject::Move(dist);
	if (m_sprite && m_sprite->m_sprite)m_sprite->m_sprite->move(dist);
}

std::shared_ptr<CustomSprite> Engine::OneSpriteDynamicObject::GetSprite() const
{
	return m_sprite;
}

void Engine::OneSpriteDynamicObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite->m_sprite, states);
}
