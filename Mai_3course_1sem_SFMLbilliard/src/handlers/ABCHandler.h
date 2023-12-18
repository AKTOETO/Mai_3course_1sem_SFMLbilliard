#ifndef ABCHANDLER_H
#define ABCHANDLER_H

#include "../ProgContext.h"
#include "../math/Vec2.h"

// Обработчики событий и логики определенных обхектов
// (мыши, клавиатуры, окна, ...)
class ABCHandler
{
public:
	ABCHandler(std::shared_ptr<ProgContext> context)
		:m_context(context)
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) { return false; };

	// Обработка логики
	virtual bool LogicHandling() { return false; };

	// Отрисовка в окне
	virtual bool DrawHandling() { return false; }

protected:
	// контекст приложения
	std::shared_ptr<ProgContext> m_context;

};

#endif
