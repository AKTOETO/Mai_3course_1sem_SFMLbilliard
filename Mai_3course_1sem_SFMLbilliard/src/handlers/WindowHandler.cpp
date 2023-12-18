#include "WindowHandler.h"

bool WindowHandler::EventHandling(const Event& evnt)
{
	switch (evnt.type)
	{
	case Event::Closed:
		//lg.Info("Exiting the program");
		m_context->m_window->close();
		break;

	case Event::Resized:
		m_context->wind_width = evnt.size.width;
		m_context->wind_height = evnt.size.height;
		//lg.Info("Window resized " + std::to_string(wind_width) + " " + std::to_string(wind_height));
		FloatRect visib_area(0, 0, m_context->wind_width, m_context->wind_height);
		m_context->m_window->setView(View(visib_area));
		break;
	}

	return false;
}

bool WindowHandler::LogicHandling()
{

	// вычисление времени прошежшего с последнего кадра
	m_cur_time = m_clock.getElapsedTime();
	m_context->m_dT = m_cur_time.asMilliseconds() - m_last_time.asMilliseconds();
	m_context->m_dT /= 800;

	// вычисление количества кадров в секунду
	m_fps = 1.f / (m_cur_time.asSeconds() - m_last_time.asSeconds());
	m_last_time = m_cur_time;
	m_context->m_window->setTitle(
		L" fps: " + std::to_wstring(floor(m_fps)) +
		L" DT: " + std::to_string(m_context->m_dT)
	);

	return false;
}

bool WindowHandler::DrawHandling()
{
	// отрисовка шаров на экране
	m_context->m_window->draw(*m_context->m_balls);

	return false;
}
