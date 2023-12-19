#include "pch.h"
#include "logger/Logger.h"
#include "handlers/IncludeAllHandlers.h"
using namespace sf;

// логгер
static Logger lg(Logger::LogMode::ConsoleAndFile);

// главная функция
int main()
{
	lg.Info("Start Program");

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

		// отрисовка линий контакта шаров
		/*for (auto& col_pair : cnxt->m_collided_balls)
		{
			Vertex line[] =
			{
				sf::Vertex(col_pair.first->getPosition()),
				sf::Vertex(col_pair.second->getPosition())
			};
			line[0].color = Color::Red;
			line[1].color = Color::Red;

			cnxt->m_window->draw(line, 2, sf::Lines);
		}*/

		cnxt->m_window->display();

	}

	return 0;
}


