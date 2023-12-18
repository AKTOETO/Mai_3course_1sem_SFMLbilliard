#include "GodModHandler.h"

GodModHandler::GodModHandler(std::shared_ptr<ProgContext> context)
	:ABCHandler(context)
{
	// загрузка необходимых шрифтов
	if (!m_charac_desc_font.loadFromFile("assets/fonts/Disket-Mono-Regular.ttf"))
	{
		//lg.Warning(std::string("Font \'") + "assets/fonts/Disket-Mono-Regular.ttf"
		//	+ "\' wasn`t loaded");
	}
}

bool GodModHandler::LogicHandling()
{
	// если включен режим бога
	if (m_context->m_god_mode)
	{
		// очистка списков
		m_text.clear();
		m_collided_lines.clear();

		// отрисовка основных характеристик каждого шара
		// если список шаров не пуст
		if (m_context->m_balls->GetBalls().size())
		{
			// берем первый шар и рисуем для него всю статистику
			auto ball = m_context->m_balls->GetBalls().begin();

			while (ball != m_context->m_balls->GetBalls().end())
			{
				// добавление массы объекта на экран
				// создание шрифта
				m_text.emplace_back(Text());

				// установка шрифта
				m_text.back().setFont(m_charac_desc_font);

				// размер шрифта
				m_text.back().setCharacterSize(10);

				// установка строки
				m_text.back().setString("mass: " + std::to_string(int((*ball)->GetMass())));

				// установка цвета текста
				m_text.back().setFillColor(Color::Black);

				// установка позиции отрисовки
				m_text.back().setPosition((*ball)->getPosition() +
					Vector2f{ (*ball)->getRadius(), -(*ball)->getRadius() });

				// добавление вектора скорости
				// создание шрифта
				m_text.emplace_back(Text());

				// установка шрифта
				m_text.back().setFont(m_charac_desc_font);

				// размер шрифта
				m_text.back().setCharacterSize(10);

				// установка строки
				m_text.back().setString("vel: " + std::to_string((*ball)->GetVelocityScal()));

				// установка цвета текста
				m_text.back().setFillColor(Color::Black);

				// установка позиции отрисовки
				m_text.back().setPosition((*ball)->getPosition() +
					Vector2f{ (*ball)->getRadius(), -(*ball)->getRadius() + 10 });

				ball++;
			}
		}

		// создание линий для пар пересекшихся шаров
		for (auto& el : m_context->m_collided_balls)
		{
			// первый шар
			auto& fball = *el.first;

			// второй шар
			auto& sball = *el.second;

			// добавление координат
			Line ln;
			ln.m_line[0] = Vertex(fball.getPosition());
			ln.m_line[1] = Vertex(sball.getPosition());

			// Устанавливаем белый цвет линий пересечения
			ln.m_line[0].color = ln.m_line[1].color = Color::White;

			// добавляем в список линий
			m_collided_lines.emplace_back(ln);
		}

	}
	else
	{
		// очистка списков
		m_text.clear();
		m_collided_lines.clear();
	}

	return false;
}

bool GodModHandler::DrawHandling()
{
	// отрисовка текста на экране рядом с мячиком
	for (auto& txt : m_text)
		m_context->m_window->draw(txt);

	// отрисовка пересекшихся пар 
	for (auto& el : m_collided_lines)
		m_context->m_window->draw(el.m_line, 2, sf::Lines);


	return false;
}
