#ifndef PHYSICHANDLER_H
#define PHYSICHANDLER_H

#include "ABCHandler.h"

// обработчик физики
class PhysicHandler
	:public ABCHandler
{
public:
	PhysicHandler(std::shared_ptr<ProgContext> context);

	// обработка логики
	virtual bool LogicHandling() override;

protected:

	// количество эпох
	// на каждое обновление кадра
	int m_sim_updates;

	// время симуляции физики в каждой эпохе
	float m_sim_elapsed_time;

	// количество возможных обновлений физики за одну эпоху
	// (это нужно для того, чтобы отследить сразу несколько возможных 
	// столкновений за эпоху)
	int m_max_sim_steps;
};

#endif
