#ifndef ONESPRITEMOVEABLEOBJECT_H
#define ONESPRITEMOVEABLEOBJECT_H

#include "../physics/DynamicObject.h"
#include "../datatypes/CustomSprite.h"

namespace Engine
{
	class OneSpriteDynamicObject :
		public DynamicObject,
		public Drawable
	{
	protected:
		// картинка, отображаемая на экране
		std::shared_ptr<CustomSprite> m_sprite;

	public:
		OneSpriteDynamicObject(int id, std::shared_ptr<CustomSprite> sprite = nullptr);

		// get/set методы
		void SetSprite(std::shared_ptr<CustomSprite> sprite);
		virtual void SetPos(Vector2f pos) override;
		virtual void Move(Vector2f dist) override;

		std::shared_ptr<CustomSprite> GetSprite() const;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	};
}

#endif // !ONESPRITEMOVEABLEOBJECT_H
