#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include "ABCHandler.h"

// обработчик клавиатуры
class KeyboardHandler
	:public ABCHandler
{
public:
	KeyboardHandler(std::shared_ptr<ProgContext> context)
		:ABCHandler(context)
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) override;

protected:
};


#endif
