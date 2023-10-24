#ifndef CUSTOMSPRITE_H
#define CUSTOMSPRITE_H

#include "../pch.h"

// структрура спрайта с текстурой
struct CustomSprite
{
	std::shared_ptr<Sprite> m_sprite;
	std::shared_ptr<Texture> m_texture;

	CustomSprite() :
		m_sprite(new Sprite), m_texture(nullptr)
	{}

	void SetTexture(std::shared_ptr<Texture> _texture)
	{
		m_texture = _texture;
		if (_texture)
			m_sprite->setTexture(*m_texture);
	}

	std::shared_ptr<Sprite> operator()() { return m_sprite; }
};

#endif // !CUSTOMSPRITE_H

