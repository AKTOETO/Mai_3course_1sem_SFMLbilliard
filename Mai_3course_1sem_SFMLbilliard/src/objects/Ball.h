#ifndef BALL_H
#define BALL_H

#include "OneSpriteMoveableObject.h"

namespace Engine
{
	// стрктура шара
	class Ball :
		public OneSpriteDynamicObject
	{
	protected:
		// радиус шара
		float m_radius;

	public:
		Ball(int id, float radius = 0.f);

		// get/set методы
		void SetRadius(float radius);

		float GetRadius() const;
	};
}

#endif
