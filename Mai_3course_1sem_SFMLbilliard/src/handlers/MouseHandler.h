#ifndef MOUSE_HANDLER_H
#define MOUSE_HANDLER_H

#include "ABCHandler.h"
#include "../objects/balls/IncludeAllBalls.h"

// обработчик мыши
class MouseHandler
	:public ABCHandler
{
public:
	MouseHandler(std::shared_ptr<ProgContext> context)
		:ABCHandler(context),
		m_moving_ball(m_context->m_balls->GetBalls().end()),
		m_shooting_vec({ .0,.0 })
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) override;

	// обработка логики
	virtual bool LogicHandling() override;

protected:
	// позиция мыши
	Vector2f m_mouse_pos;

	// выбранный шар для перемещения с удара
	std::list<std::shared_ptr<Ball>>::const_iterator m_moving_ball;

	// вектор удара по шару
	Vector2f m_shooting_vec;

	// получение координат мыши в окне
	Vector2f GetMousePos();
};

#endif // !MOUSE_HANDLER_H
