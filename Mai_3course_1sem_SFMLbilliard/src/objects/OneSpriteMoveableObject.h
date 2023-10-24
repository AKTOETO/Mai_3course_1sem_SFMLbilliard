#ifndef ONESPRITEMOVEABLEOBJECT_H
#define ONESPRITEMOVEABLEOBJECT_H

#include "MoveableObject.h"
#include "../datatypes/CustomSprite.h"

namespace Engine
{
	class OneSpriteMoveableObject :
		public MoveableObject,
		public Drawable
	{
	protected:
		// картинка, отображаемая на экране
		std::shared_ptr<CustomSprite> m_sprite;
	public:
		OneSpriteMoveableObject(int id, std::shared_ptr<CustomSprite> sprite = nullptr) :
			MoveableObject(id), m_sprite(sprite)
		{}

		// get/set методы
		void SetSprite(std::shared_ptr<CustomSprite> sprite) { m_sprite = sprite; }

		std::shared_ptr<CustomSprite> GetSprite() const { return m_sprite; }
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(*m_sprite->m_sprite, states);
		}
	};
}

#endif // !ONESPRITEMOVEABLEOBJECT_H
