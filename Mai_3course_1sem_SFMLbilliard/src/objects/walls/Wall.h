#ifndef WALL_H
#define WALL_H

#include "../../pch.h"
#include "../../math/Vec2.h"

class Wall : public Drawable
{
protected:
	// координаты начала
	Vector2f m_start;

	// координаты конца
	Vector2f m_end;

	// радиус
	float m_radius;

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//target.draw(m_sprite, states);
	}
public:
	Wall(Vector2f start = { 10.f,50.f },
		Vector2f end = { 100.f,20.f },
		float radius = 10);

};

#endif
