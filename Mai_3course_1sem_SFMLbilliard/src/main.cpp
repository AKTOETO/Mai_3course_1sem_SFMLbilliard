#include "pch.h"
#include "logger/Logger.h"

using namespace sf;

static int wind_width = 1200;//500;
static int wind_height = 800;//100;

static Vector2i wind_center(wind_width / 2, wind_height / 2);

// случайная координата на экране
Vector2f GetRandWindowPoint()
{
	return Vector2f(rand() % wind_width, rand() % wind_height);
}

// логгер
static Logger lg(Logger::LogMode::ConsoleAndFile);

// нормализация вектора
Vector2f NormalizeVector(Vector2f _vect)
{
	float norm = sqrt(_vect.x * _vect.x + _vect.y * _vect.y);
	return _vect / norm;
}

// отрисовываемый шар 
class Ball : public CircleShape
{
protected:
	// радиус
	float m_radius;

	// толщина обводки внешней
	float m_outline_thickness;

	// скорость
	Vector2f m_vel;

	// ускорение
	Vector2f m_acc;

	// номер шара
	int m_id;

public:
	Ball(int id, float radius = 10, Vector2f pos = { 0,0 },
		Vector2f vel = { 0,0 }, Vector2f acc = { 0,0 });
	~Ball() {};

	// Get/Set методы
	const Vector2f& GetVelocity()const { return m_vel; }
	const Vector2f& GetAcceleration()const { return m_acc; }
	const int& GetId()const { return m_id; }

	void SetVelocity(const Vector2f& vel) { m_vel = vel; }
	void SetAcceleration(const Vector2f& acc) { m_acc = acc; }

	// проверка пересечения с другим шаром
	bool IsIntersected(const Ball& ball);

	// нахождение расстояния между центрами двух шаров
	float CenterDist(const Ball& ball);

	// нахождение расстояния по оси x
	float GetXDist(const Ball& ball);

	// нахождение расстояния по оси y
	float GetYDist(const Ball& ball);

	// полчение радиуса
	float getRadius() const { return m_radius; }
};

Ball::Ball(int id, float radius, Vector2f pos,
	Vector2f vel, Vector2f acc)
	:CircleShape(radius), m_id(id), m_vel(vel), m_acc(acc),
	m_outline_thickness(5)
{
	m_radius = (radius + m_outline_thickness);
	setPosition(pos);
	setOutlineColor(Color::Black);
	setOutlineThickness(m_outline_thickness);
	setOrigin({ radius, radius });
}

bool Ball::IsIntersected(const Ball& ball)
{
	// расстоние между центрами
	float center_dist = CenterDist(ball);

	// если расстояние между центрами равно сумме радиусов,
	// то шары пересеклись
	return center_dist <= this->getRadius() + ball.getRadius();
}

float Ball::CenterDist(const Ball& ball)
{
	// Для начала следует найти расстояние между центром шаров
	return	sqrtf(powf(GetXDist(ball), 2) + powf(GetYDist(ball), 2));
}

float Ball::GetXDist(const Ball& ball)
{
	return this->getGlobalBounds().left - ball.getGlobalBounds().left;
}

float Ball::GetYDist(const Ball& ball)
{
	return this->getGlobalBounds().top - ball.getGlobalBounds().top;
}

// Синий шар
class BlueBall : public Ball
{
public:
	BlueBall(int id, Vector2f pos = { 50,50 });
};

BlueBall::BlueBall(int id, Vector2f pos)
	:Ball(id, 10, pos)
{
	setFillColor(Color::Blue);
}

// Список шаров
class BallsVector : public Drawable
{
protected:
	std::list<std::shared_ptr<Ball>> m_balls;

public:
	// добавить синий шар в случайное место
	void AddBlueBallAtRandPos()
	{
		m_balls.push_back(std::make_shared<BlueBall>(0, GetRandWindowPoint()));
	}

	// добавить шар
	void AddBall(const std::shared_ptr<Ball>& ball)
	{
		// перебираем все шары и смотрим, есть ли пересечение
		// с тем, что мы хотим добавить
		for (auto& mball : m_balls)
		{
			// если шар пересекается с каким-либо уже существующим,
			// то просто выходим из функции, не добваляя новый шар
			if (ball->IsIntersected(*mball))
				return;
		}

		// если же шар не пересекся с другими, добавляем его в список шаров
		m_balls.emplace_back(ball);
		lg.Info("ball spawned at:" +
			std::to_string(int(ball->getGlobalBounds().left)) + " " +
			std::to_string(int(ball->getGlobalBounds().top))
		);
	}

	// удалить шар
	void DeleteBall(std::list<std::shared_ptr<Ball>>::const_iterator iter)
	{
		m_balls.erase(iter);
	}

	// получить список шаров
	const std::list<std::shared_ptr<Ball>>& GetBalls() const { return m_balls; }

	// нарисовать все шары
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_balls.empty())
			// рисуем каждый шар
			for (auto& ball : m_balls)
				target.draw(*ball, states);
	}
};

//// класс кнопки (trigger)
//template<class Ty>
//class Button
//{
//public:
//
//	// указатель на функцию
//	using pfunc = std::function<void()>;
//
//	// перечисление состояний
//	enum class State
//	{
//		NONE = 0,
//		PRESSED,	// нажата
//		RELEASED,	// отпущена
//		HOLDED		// нажата и не отпущена
//	};
//
//	// конструктор
//	Button(Ty key)
//		:m_state(State::NONE), m_key_code(key),
//		m_pressed_func(nullptr), m_released_func(nullptr),
//		m_holded_func(nullptr)
//	{}
//
//	// обработка состояния кнопки на клавиатуре
//	State Logic();
//
//	// Установка новой кнопки
//	void SetKeyCode(Ty key) { m_key_code = key; }
//
//	// Получение текущей кнопки
//	Ty GetKeyCode() const { return m_key_code; }
//
//	// Установка функций обработки состояний
//	// нажатие
//	void SetPressedFunc(pfunc func) { m_pressed_func = func; }
//
//	// удержание
//	void SetHoldedFunc(pfunc func) { m_holded_func = func; }
//
//	// отпускание
//	void SetReleasedFunc(pfunc func) { m_released_func = func; }
//
//protected:
//
//	// проверка нажатия кнопки
//	virtual bool IsKeyPressed() = 0;
//
//	// текущее состояние кнопки
//	State m_state;
//
//	// клавиша, на которую срабатывает кнопка
//	Ty m_key_code;
//
//	// функция, которая вызывается при нажатии
//	pfunc m_pressed_func;
//
//	// функция, которая вызывается при зажатии
//	pfunc m_holded_func;
//
//	// функция, которая вызывается при отпускании
//	pfunc m_released_func;
//};
//
//template<class Ty>
//Button<Ty>::State Button<Ty>::Logic()
//{
//	// если кнопка нажата
//	if (IsKeyPressed()
//		&& m_state == State::NONE)
//	{
//		m_state = State::PRESSED;
//		//lg.Info("pressed");
//		if (m_pressed_func)m_pressed_func();
//	}
//	// если кнопка зажата
//	else if (IsKeyPressed() && m_state == State::PRESSED)
//	{
//		m_state = State::HOLDED;
//		//lg.Info("holded");
//		if (m_holded_func)m_holded_func();
//	}
//	// если кнопка отжата
//	else if (!IsKeyPressed() &&
//		(
//			m_state == State::PRESSED ||
//			m_state == State::HOLDED
//			))
//	{
//		m_state = State::RELEASED;
//		//lg.Info("released");
//		if (m_released_func)m_released_func();
//	}
//	// если кнопка уже давно отжата
//	else if (!IsKeyPressed() && m_state == State::RELEASED)
//	{
//		//lg.Info("none");
//		m_state = State::NONE;
//	}
//
//	return m_state;
//}
//
//// Обработка кнопки с клавиатуры
//class KeyboardButton :
//	public Button<Keyboard::Key>
//{
//public:
//	KeyboardButton(Keyboard::Key key)
//		:Button(key)
//	{}
//
//protected:
//
//	// проверка нажатия кнопки
//	virtual bool IsKeyPressed() override;
//};
//
//// Перегрузка функции обработки нажатия кнопки
//bool KeyboardButton::IsKeyPressed()
//{
//	return Keyboard::isKeyPressed(m_key_code);
//}
//
//// Обработка кнопки с мыши
//class MouseButton
//	: public Button<Mouse::Button>
//{
//public:
//	MouseButton(Mouse::Button key)
//		:Button(key)
//	{}
//
//protected:
//	// проверка нажатия кнопки
//	virtual bool IsKeyPressed() override;
//};
//
//// Перегрузка функции нажатия клавиши на мыши
//bool MouseButton::IsKeyPressed()
//{
//	return Mouse::isButtonPressed(m_key_code);
//}

// контекст программы
struct ProgContext
{
	// время рендера последнего кадра
	float m_dT;

	// выбранный шар для перемещения с помощью колесика
	std::list<std::shared_ptr<Ball>>::const_iterator m_following_ball;

	// указатель на окно
	std::unique_ptr<RenderWindow> m_window;

	// указатель на массив с шарами
	std::unique_ptr<BallsVector> m_balls;

	// конструктор
	ProgContext()
		:m_dT(.0)
	{
		// настройки окна
		sf::ContextSettings settings;
		settings.depthBits = 4;
		settings.stencilBits = 8;
		settings.antialiasingLevel = 16;
		settings.majorVersion = 3;
		settings.minorVersion = 0;

		// создание окна
		m_window = std::make_unique<RenderWindow>(VideoMode(wind_width, wind_height), "SFML works!", Style::Default, settings);

		// перемещение окна
		m_window->setPosition({ 50,50 });
		// лимит частоты кадров 60
		m_window->setFramerateLimit(60);

		// создание массива с шарами
		m_balls = std::make_unique<BallsVector>();

		// определение шара нулевым значением
		m_following_ball = m_balls->GetBalls().end();
	}
};

// Обработчики событий и логики определенных обхектов
// (мыши, клавиатуры, окна, ...)
class ABCHandler
{
public:
	ABCHandler(std::shared_ptr<ProgContext> context)
		:m_context(context)
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) = 0;

	// Обработка логики
	virtual bool LogicHandling() = 0;

protected:
	// контекст приложения
	std::shared_ptr<ProgContext> m_context;

};

// обработчик мыши
class MouseHandler
	:public ABCHandler
{
public:
	MouseHandler(std::shared_ptr<ProgContext> context)
		:ABCHandler(context),
		m_moving_ball(m_context->m_balls->GetBalls().end()),
		m_shooting_vec({ .0,.0 })
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) override;

	// обработка логики
	virtual bool LogicHandling() override;

protected:
	// позиция мыши
	Vector2f m_mouse_pos;

	// выбранный шар для перемещения с удара
	std::list<std::shared_ptr<Ball>>::const_iterator m_moving_ball;

	// вектор удара по шару
	Vector2f m_shooting_vec;

	// получение координат мыши в окне
	Vector2f GetMousePos();
};

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
					lg.Info("shoot ball");
					// выбираем шар котрым выстрелим
					m_moving_ball = ball;
					break;
				}
				else ++ball;
			}

			// если мы не навели курсор на шар при нажатии лкм,
			// то надо заспавнить новый шар
			if (ball == m_context->m_balls->GetBalls().end())
			{
				m_context->m_balls->AddBall(std::make_shared<BlueBall>(0, m_mouse_pos));
			}
			break;

			// удаление шара, при нажатии на пкм
		case Mouse::Button::Right:

			// перебираем все шары, и смотрим, 
			ball = m_context->m_balls->GetBalls().begin();
			while (ball != m_context->m_balls->GetBalls().end())
			{
				// есть ли пересечение мыши с каким-нибудь
				if ((*ball)->getGlobalBounds().contains(m_mouse_pos))
				{
					lg.Info("delete ball");
					// удаляем текущий шар
					m_context->m_balls->DeleteBall(ball++);
				}
				else ++ball;
			}

			break;

			// выбор шара, который буду двигать
		case Mouse::Button::Middle:

			// если шар не выбран
			if (m_context->m_following_ball == m_context->m_balls->GetBalls().end())
			{
				// перебираем все шары, и смотрим, 
				ball = m_context->m_balls->GetBalls().begin();
				while (ball != m_context->m_balls->GetBalls().end())
				{
					// есть ли пересечение мыши с каким-нибудь
					if ((*ball)->getGlobalBounds().contains(m_mouse_pos))
					{
						lg.Info("ball captrued for moving");
						// захватываю шар
						m_context->m_following_ball = ball;
						break;
					}
					else ++ball;
				}
			}
			break;

		default:
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
				lg.Info("release captured ball");
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
				m_shooting_vec = m_mouse_pos - (*m_moving_ball)->getPosition();

				lg.Info("shoot the ball by: " + std::to_string(m_shooting_vec.x) + " " + std::to_string(m_shooting_vec.y));

				// говорим, что больше нет выбранного шара для выстрела
				m_moving_ball = m_context->m_balls->GetBalls().end();
			}

			break;


		default:
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

	// обработка логики
	virtual bool LogicHandling() override;

protected:
};

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
			lg.Info("Hitted Esc - exiting the program");
			m_context->m_window->close();
			break;

		case Keyboard::Key::A:
			m_context->m_balls->AddBlueBallAtRandPos();
			lg.Info("Adding random positioned blue ball");
			break;

		default:
			break;
		}
		break;
	default:
		break;
	}

	return false;
}

bool KeyboardHandler::LogicHandling()
{
	return false;
}

// обработчик окна
class WindowHandler
	:public ABCHandler
{
public:
	WindowHandler(std::shared_ptr<ProgContext> context)
		:ABCHandler(context), m_last_time(m_clock.getElapsedTime()), m_fps(.0)
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) override;

	// обработка логики
	virtual bool LogicHandling() override;

protected:
	// для расчета фпс и времени кадра
	sf::Clock m_clock;
	Time m_last_time;
	Time m_cur_time;
	float m_fps;
};

bool WindowHandler::EventHandling(const Event& evnt)
{
	switch (evnt.type)
	{
	case Event::Closed:
		lg.Info("Exiting the program");
		m_context->m_window->close();
		break;

	case Event::Resized:
		wind_width = evnt.size.width;
		wind_height = evnt.size.height;
		lg.Info("Window resized " + std::to_string(wind_width) + " " + std::to_string(wind_height));
		FloatRect visib_area(0, 0, wind_width, wind_height);
		m_context->m_window->setView(View(visib_area));
		break;
	}

	return false;
}

bool WindowHandler::LogicHandling()
{
	// вычисление времени прошежшего с последнего кадра
	m_cur_time = m_clock.getElapsedTime();
	m_context->m_dT = m_cur_time.asMicroseconds();
	m_context->m_dT /= 800;

	// вычисление количества кадров в секунду
	m_fps = 1.f / (m_cur_time.asSeconds() - m_last_time.asSeconds());
	m_last_time = m_cur_time;
	m_context->m_window->setTitle(
		L" fps: " + std::to_wstring(floor(m_fps))
	);

	return false;
}

// обработчик физики
class PhysicHandler
	:public ABCHandler
{
public:
	PhysicHandler(std::shared_ptr<ProgContext> context)
		:ABCHandler(context)
	{}

	// обработка событий
	virtual bool EventHandling(const Event&) override;

	// обработка логики
	virtual bool LogicHandling() override;

protected:
};

bool PhysicHandler::EventHandling(const Event&)
{
	return false;
}

bool PhysicHandler::LogicHandling()
{
	// проверка на столкновение шаров друг с другом

	// итератор для перебора первого шара
	std::list<std::shared_ptr<Ball>>::const_iterator source = m_context->m_balls->GetBalls().begin();


	// проходимся по всем шарам и пытаемся пересечь их друг с другом
	while (source != m_context->m_balls->GetBalls().end())
	{
		// итератор для перебора второго шара
		std::list<std::shared_ptr<Ball>>::const_iterator target = m_context->m_balls->GetBalls().begin();

		// проходимся по всем шарам и пытаемся пересечь их друг с другом
		while (target != m_context->m_balls->GetBalls().end())
		{
			// если это разные шары
			if (source != target)
			{
				// проверяем их на пересечение
				if ((*source)->IsIntersected(**target))
				{
					// статическая коллизия
					// мы вытесняем один шар из другого

					// сохраним расстояние между центрами шаров по разным осям
					float x_dist = (*source)->GetXDist(**target);
					float y_dist = (*source)->GetYDist(**target);

					// Для начала следует найти расстояние между центром шаров
					float center_dist = (*source)->CenterDist(**target);

					// расстояние, на которое они пересекаются
					// на это расстояние мы сдвинем каждый шар, поэтому делим его на 2
					float overlap_dist = 0.5 * (center_dist - (*source)->getRadius() - (*target)->getRadius());

					// сдвигаем шары друг относительно друга

					// если шаром не управляют мышкой, то двигаем его
					if (source != m_context->m_following_ball)
						(*source)->move(Vector2f(
							-overlap_dist * (x_dist) / center_dist,
							-overlap_dist * (y_dist) / center_dist
						));

					// если шаром не управляют мышкой, то двигаем его
					if (target != m_context->m_following_ball)
						(*target)->move(Vector2f(
							overlap_dist * (x_dist) / center_dist,
							overlap_dist * (y_dist) / center_dist
						));

				}
			}
			target++;
		}
		source++;
	}

	return false;
}

// главная функция
int main()
{
	lg.Info("Start Program");

	srand(time(NULL));

	// создание контекста приложения
	std::shared_ptr<ProgContext> cnxt = std::make_shared<ProgContext>();

	// управляющий мышью
	MouseHandler h_mouse(cnxt);

	// управляющий клавиатурой
	KeyboardHandler h_keyboard(cnxt);

	// управляющий окном
	WindowHandler h_window(cnxt);

	// обработчик физики
	PhysicHandler h_phys(cnxt);

	// цикл программы
	while (cnxt->m_window->isOpen())
	{
		// событие
		Event evnt;

		// обработка событий
		while (cnxt->m_window->pollEvent(evnt))
		{
			// обработка событий мыши
			h_mouse.EventHandling(evnt);

			// обработка событий клавиатуры
			h_keyboard.EventHandling(evnt);

			// обработка событий окна
			h_window.EventHandling(evnt);

			// обработка событий физики 
			// (на самом деле таких нет. Добавил просто для красоты)
			h_phys.EventHandling(evnt);
		}

		// обработка логики	мыши
		h_mouse.LogicHandling();

		// обработка логики	клавиатуры
		h_keyboard.LogicHandling();

		// обработка логики	окна
		h_window.LogicHandling();

		// обработка логики	физики
		h_phys.LogicHandling();


		// отрисовка всего
		cnxt->m_window->clear(Color::Cyan);

		cnxt->m_window->draw(*cnxt->m_balls);

		cnxt->m_window->display();
	}

	return 0;
}

