#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include "ABCHandler.h"

// обработчик окна
class WindowHandler
	:public ABCHandler
{
public:
	WindowHandler(std::shared_ptr<ProgContext> context)
		:ABCHandler(context), m_last_time(m_clock.getElapsedTime()), m_fps(.0)
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) override;

	// обработка логики
	virtual bool LogicHandling() override;

	// Отрисовка в окне
	virtual bool DrawHandling() override;

protected:
	// для расчета фпс и времени кадра
	sf::Clock m_clock;
	Time m_last_time;
	Time m_cur_time;
	float m_fps;
};


#endif
