#ifndef BALL_H
#define BALL_H

#include "OneSpriteMoveableObject.h"

namespace Engine
{
	// стрктура шара
	class Ball :
		public OneSpriteMoveableObject
	{
	protected:
		// радиус шара
		float m_radius;

	public:
		Ball(int id, float radius = 0.f):
			OneSpriteMoveableObject(id), m_radius(radius)
		{}

		// get/set методы
		float GetRadius() const { return m_radius; }

		void SetRadius(float radius) { m_radius = radius; }
	};
}

#endif
