#ifndef WALLLIST_H
#define WALLLIST_H

#include "../objects/walls/Wall.h"

// TODO:
// 1) можно сделать АБК класса
// ObjectList<object>, чтобы можно было управлять 
// созданием шаров, стен, итд...
// Унаследоваться от этого интерфейса и сделать 
// WallList и BallList
// 2) при добавлении АБС сделать object тоже АБК
// У которого есть метод IsIntersected


// список стен
class WallList : public Drawable
{
protected:
	std::list<std::shared_ptr<Wall>> m_walls;

public:

	// добавить стену
	void AddWall(const std::shared_ptr<Wall>& wall)
	{
		// перебираем все шары и смотрим, есть ли пересечение
		// с тем, что мы хотим добавить
		//for (auto& mball : m_walls)
		//{
		//	// если шар пересекается с каким-либо уже существующим,
		//	// то просто выходим из функции, не добваляя новый шар
		//	if (ball->IsIntersected(*mball))
		//		return;
		//}

		// если же шар не пересекся с другими, добавляем его в список шаров
		m_walls.emplace_back(wall);
		//lg.Info("ball spawned at:" +
		//	std::to_string(int(ball->getPosition().x)) + " " +
		//	std::to_string(int(ball->getPosition().y))
		//);
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
