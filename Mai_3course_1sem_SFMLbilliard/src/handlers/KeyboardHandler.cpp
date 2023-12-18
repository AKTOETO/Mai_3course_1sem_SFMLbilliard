﻿#include "KeyboardHandler.h"

bool KeyboardHandler::EventHandling(const Event& evnt)
{
	switch (evnt.type)
	{
		// если нажата кнопка на клавиатуре
	case Event::KeyPressed:

		switch (evnt.key.code)
		{
			// если нажат esc - выходим из программы
		case Keyboard::Key::Escape:
			//lg.Info("Hitted Esc - exiting the program");
			m_context->m_window->close();
			break;

			// добавить синий шар в случайное место
		case Keyboard::Key::A:
			if (m_context->m_god_mode)
			{
				m_context->m_balls->AddBall(
					std::make_shared<BlueBall>(
						Vector2f(GetWindowRandPoint(*m_context->m_window))
					)
				);
				//lg.Info("Adding random positioned blue ball");
			}
			break;
			
			// добавить красный шар в случайное место
		case Keyboard::Key::S:
			if (m_context->m_god_mode)
			{
				m_context->m_balls->AddBall(
					std::make_shared<RedBall>(
						Vector2f(GetWindowRandPoint(*m_context->m_window))
					)
				);
				//lg.Info("Adding random positioned blue ball");
			}
			break;


			// переключение режима бога
		case Keyboard::Key::F1:
			m_context->m_god_mode = !m_context->m_god_mode;
			//if (m_context->m_god_mode)
				//lg.Warning("God mode activated");
			//else
				//lg.Warning("God mode disactivated");
			break;
		}
		break;
	}

	return false;
}
