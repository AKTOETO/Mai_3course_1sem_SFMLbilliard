﻿#ifndef BALLS_LIST_H
#define BALLS_LIST_H

#include "../objects/balls/IncludeAllBalls.h"

// Список шаров
class BallList : public Drawable
{
protected:
	std::list<std::shared_ptr<Ball>> m_balls;

public:

	// добавить шар
	void AddBall(const std::shared_ptr<Ball>& ball)
	{
		// перебираем все шары и смотрим, есть ли пересечение
		// с тем, что мы хотим добавить
		for (auto& mball : m_balls)
		{
			// если шар пересекается с каким-либо уже существующим,
			// то просто выходим из функции, не добваляя новый шар
			if (ball->IsIntersected(*mball))
				return;
		}

		// если же шар не пересекся с другими, добавляем его в список шаров
		m_balls.emplace_back(ball);
	}

	// удалить шар
	void DeleteBall(std::list<std::shared_ptr<Ball>>::const_iterator iter)
	{
		m_balls.erase(iter);
	}

	// получить список шаров
	const std::list<std::shared_ptr<Ball>>& GetBalls() const { return m_balls; }

	// нарисовать все шары
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_balls.empty())
			// рисуем каждый шар
			for (auto& ball : m_balls)
				target.draw(*ball, states);
	}
};

#endif
