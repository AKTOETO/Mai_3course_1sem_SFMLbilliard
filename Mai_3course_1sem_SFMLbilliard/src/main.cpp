#include "pch.h"
#include "logger/Logger.h"
#include "handlers/IncludeAllHandlers.h"
using namespace sf;

// главная функция
int main()
{
	srand(time(NULL));

	// создание контекста приложения
	std::shared_ptr<ProgContext> cnxt = std::make_shared<ProgContext>();

	// список обработчиков
	std::list<std::shared_ptr<ABCHandler>> handlers =
	{
		std::make_shared<MouseHandler>(cnxt),
		std::make_shared<KeyboardHandler>(cnxt),
		std::make_shared<WindowHandler>(cnxt),
		std::make_shared<PhysicHandler>(cnxt),
		std::make_unique<GodModHandler>(cnxt)
	};

	// цикл программы
	while (cnxt->m_window->isOpen())
	{
		// событие
		Event evnt;

		// обработка событий
		while (cnxt->m_window->pollEvent(evnt))
		{
			// обработка событий
			for (auto& el : handlers)
				el->EventHandling(evnt);
		}

		// обработка логики
		for (auto& el : handlers)
			el->LogicHandling();

		// отрисовка всего
		cnxt->m_window->clear(Color::Cyan);

		// отрисовка
		for (auto& el : handlers)
			el->DrawHandling();

		cnxt->m_window->display();

	}

	return 0;
}


