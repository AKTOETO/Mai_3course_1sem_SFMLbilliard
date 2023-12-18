#include "MouseHandler.h"

bool MouseHandler::EventHandling(const Event& evnt)
{
	// указатель на шар для перебора в цикле
	std::list<std::shared_ptr<Ball>>::const_iterator ball;

	// обработка мыши
	switch (evnt.type)
	{
		// Если нажата кнопка на мышке
	case Event::MouseButtonPressed:

		// позиция мыши в окне
		m_mouse_pos = GetMousePos();

		switch (evnt.mouseButton.button)
		{
			// нажата левая кнопка мыши
		case Mouse::Button::Left:

			// перебираем все шары, и смотрим, 
			ball = m_context->m_balls->GetBalls().begin();
			while (ball != m_context->m_balls->GetBalls().end())
			{
				// есть ли пересечение мыши с каким-нибудь
				if ((*ball)->getGlobalBounds().contains(m_mouse_pos))
				{
					// выбираем шар котрым выстрелим
					m_moving_ball = ball;
					break;
				}
				else ++ball;
			}

			// если мы не навели курсор на шар при нажатии лкм,
			// то надо заспавнить новый шар (если есть режим бога)
			if (m_context->m_god_mode &&
				ball == m_context->m_balls->GetBalls().end())
			{
				m_context->m_balls->AddBall(std::make_shared<BlueBall>(m_mouse_pos));
			}
			break;

			// удаление шара, при нажатии на пкм
		case Mouse::Button::Right:

			// перебираем все шары, и смотрим, (если есть режим бога)
			if (m_context->m_god_mode)
			{
				ball = m_context->m_balls->GetBalls().begin();
				while (ball != m_context->m_balls->GetBalls().end())
				{
					// есть ли пересечение мыши с каким-нибудь
					if ((*ball)->getGlobalBounds().contains(m_mouse_pos))
					{
						// удаляем текущий шар
						m_context->m_balls->DeleteBall(ball++);
					}
					else ++ball;
				}
			}
			break;

			// выбор шара, который буду двигать
		case Mouse::Button::Middle:

			// (если есть режим бога)
			// если шар не выбран
			if (m_context->m_god_mode &&
				m_context->m_following_ball == m_context->m_balls->GetBalls().end())
			{
				// перебираем все шары, и смотрим, 
				ball = m_context->m_balls->GetBalls().begin();
				while (ball != m_context->m_balls->GetBalls().end())
				{
					// есть ли пересечение мыши с каким-нибудь
					if ((*ball)->getGlobalBounds().contains(m_mouse_pos))
					{
						// захватываю шар
						m_context->m_following_ball = ball;
						break;
					}
					else ++ball;
				}
			}
			break;
		}
		break;

		// Если отпущена кнопка на мышке
	case Event::MouseButtonReleased:

		switch (evnt.mouseButton.button)
		{
			// отпукаем выбранный ранее шар
		case Mouse::Button::Middle:

			// если шар был выбран
			if (m_context->m_following_ball != m_context->m_balls->GetBalls().end())
			{
				// говорим, что шар больше не выбран
				m_context->m_following_ball = m_context->m_balls->GetBalls().end();
			}
			break;

			// отпущена левая кнопка мыши
		case Mouse::Button::Left:

			// если был выбран шар для выстрела
			if (m_moving_ball != m_context->m_balls->GetBalls().end())
			{
				// запоминаем текущую позицию мыши и
				// выстреливаем шаром в зависимости от направления вектора
				m_mouse_pos = GetMousePos();

				// вычисление вектора удара
				m_shooting_vec = -(m_mouse_pos - (*m_moving_ball)->getPosition());

				// установка скорости для шара
				(*m_moving_ball)->SetVelocity(2.f * m_shooting_vec);

				// говорим, что больше нет выбранного шара для выстрела
				m_moving_ball = m_context->m_balls->GetBalls().end();
			}
			break;
		}
		break;
	}

	return false;
}

bool MouseHandler::LogicHandling()
{
	// если шар был захвачен
	if (m_context->m_following_ball != m_context->m_balls->GetBalls().end())
	{
		// переносим шар, который был захвачен
		(*m_context->m_following_ball)->setPosition(GetMousePos());
	}

	return false;
}

Vector2f MouseHandler::GetMousePos()
{
	return Vector2f(Mouse::getPosition(*m_context->m_window));
}
