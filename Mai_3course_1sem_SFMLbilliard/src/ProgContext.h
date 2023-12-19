#ifndef PROGCONTEXT_H
#define PROGCONTEXT_H

#include "pch.h"
#include "objects_list/BallList.h"
#include "logger/Logger.h"

// контекст программы
struct ProgContext
{
	// размеры окна
	int wind_width;
	int wind_height;

	// режим бога
	bool m_god_mode;

	// время рендера последнего кадра
	float m_dT;

	// указатель на окно
	std::unique_ptr<RenderWindow> m_window;

	// логгер
	std::unique_ptr<Logger> m_logger;

	////////////
	//	ШАРЫ
	////////////

	// указатель на массив с шарами
	std::unique_ptr<BallList> m_balls;

	// выбранный шар для перемещения с помощью колесика
	std::list<std::shared_ptr<Ball>>::const_iterator m_following_ball;

	// список пар пересекшихся шаров
	std::list<std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>>> m_collided_balls;

	// конструктор
	ProgContext()
		:m_dT(.0), m_god_mode(1),
		wind_width(800), wind_height(600)
	{
		// создание логгера
		m_logger = std::make_unique<Logger>(Logger::LogMode::ConsoleAndFile);

		// настройки окна
		sf::ContextSettings settings;
		settings.depthBits = 4;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 16;
		settings.majorVersion = 3;
		settings.minorVersion = 0;

		// создание окна
		m_window = std::make_unique<RenderWindow>(
			VideoMode(wind_width, wind_height),
			"SFML works!", Style::Default, settings);

		// перемещение окна
		m_window->setPosition({ 0,0 });
		// лимит частоты кадров 60
		m_window->setFramerateLimit(60);

		// создание массива с шарами
		m_balls = std::make_unique<BallList>();

		// определение шара нулевым значением
		m_following_ball = m_balls->GetBalls().end();
	}
};
// макросы для удобного использования логгера
#define INFO(msg) m_context->m_logger->Info(msg)
#define WARN(msg) m_context->m_logger->Warning(msg)
#define ERRO(msg) m_context->m_logger->Error(msg)

#endif
