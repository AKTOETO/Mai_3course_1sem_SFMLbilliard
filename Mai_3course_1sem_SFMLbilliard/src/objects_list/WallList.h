#ifndef WALLLIST_H
#define WALLLIST_H

#include "../objects/walls/Wall.h"

// список стен
class WallList : public Drawable
{
protected:
	std::list<std::shared_ptr<Wall>> m_walls;

public:

	// добавить стену
	void AddWall(const std::shared_ptr<Wall>& wall)
	{
		// если же шар не пересекся с другими, добавляем его в список шаров
		m_walls.emplace_back(wall);
	}

	// удалить стену
	void DeleteWall(std::list<std::shared_ptr<Wall>>::const_iterator iter)
	{
		m_walls.erase(iter);
	}

	// получить список стен
	const std::list<std::shared_ptr<Wall>>& GetWalls() const { return m_walls; }

	// нарисовать все шары
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_walls.empty())
			// рисуем каждый шар
			for (auto& ball : m_walls)
				target.draw(*ball, states);
	}

};

#endif
