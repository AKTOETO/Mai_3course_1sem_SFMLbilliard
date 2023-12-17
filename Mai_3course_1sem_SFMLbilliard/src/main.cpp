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

// скалярное произведение двух векторов
Vector2f DotProduct(Vector2f vec1, Vector2f vec2)
{
	return { vec1.x * vec2.x, vec1.y * vec2.y };
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

	// масса
	float m_mass;

public:
	Ball(float radius = 10, Vector2f pos = { 0,0 },
		Vector2f vel = { 0,0 }, Vector2f acc = { 0,0 });
	~Ball() {};

	// Get/Set методы
	const Vector2f& GetVelocity() const { return m_vel; }
	const float& GetVelocityScal() const { return sqrt(m_vel.x * m_vel.x + m_vel.y * m_vel.y); }
	const Vector2f& GetAcceleration() const { return m_acc; }
	const float& GetMass() const { return m_mass; }

	void SetVelocity(const Vector2f& vel)
	{
		//lg.Info("current velocity: " + std::to_string(GetVelocityScal()));
		m_vel = vel;
	}
	void SetAcceleration(const Vector2f& acc) { m_acc = acc; }

	void IncreaseVelocity(const Vector2f& vel) { m_vel += vel; }
	void IncreaseAcceleration(const Vector2f& acc) { m_acc += acc; }

	// проверка пересечения с другим шаром
	bool IsIntersected(const Ball& ball);

	// нахождение расстояния между центрами двух шаров
	float GetCenterDist(const Ball& ball);

	// нахождение расстояния по оси x
	float GetXDist(const Ball& ball);

	// нахождение расстояния по оси y
	float GetYDist(const Ball& ball);

	// полчение радиуса
	float getRadius() const { return m_radius; }
};

Ball::Ball(float radius, Vector2f pos,
	Vector2f vel, Vector2f acc)
	:CircleShape(radius), m_vel(vel), m_acc(acc),
	m_outline_thickness(5)
{
	// радиус учитывает толщину обводки
	m_radius = (radius + m_outline_thickness);
	// масса зависит от радиуса
	m_mass = m_radius * 5.f;

	setPosition(pos);
	setOutlineColor(Color::Black);
	setOutlineThickness(m_outline_thickness);
	setOrigin({ radius, radius });
	
	lg.Info("center in: " + std::to_string(m_radius / 2) + " " + std::to_string(m_radius / 2) +
		" radius: " + std::to_string(m_radius));
}

bool Ball::IsIntersected(const Ball& ball)
{
	// расстоние между центрами
	float center_dist = GetCenterDist(ball);

	// если расстояние между центрами равно сумме радиусов,
	// то шары пересеклись
	return fabs(center_dist) <= this->getRadius() + ball.getRadius();
}

float Ball::GetCenterDist(const Ball& ball)
{
	// Для начала следует найти расстояние между центром шаров
	return sqrtf(powf(GetXDist(ball), 2) + powf(GetYDist(ball), 2));
}

float Ball::GetXDist(const Ball& ball)
{
	return abs(this->getPosition().x// + this->getRadius()
		- ball.getPosition().x //+ ball.getRadius()
	);
}

float Ball::GetYDist(const Ball& ball)
{
	return abs(this->getPosition().y// + this->getRadius()
		- ball.getPosition().y //+ ball.getRadius()
	);
}

// Синий шар
class BlueBall : public Ball
{
public:
	BlueBall(Vector2f pos = { 50,50 });
};

BlueBall::BlueBall(Vector2f pos)
	:Ball(10, pos)
{
	setFillColor(Color::Blue);
}

// Красный шар
class RedBall : public Ball
{
public:
	RedBall(Vector2f pos = { 50,50 });
};

RedBall::RedBall( Vector2f pos)
	:Ball(30, pos)
{
	setFillColor(Color::Red);
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
		m_balls.push_back(std::make_shared<RedBall>(GetRandWindowPoint()));
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

	// список пар пересекшихся шаров
	std::list<std::pair<std::shared_ptr<Ball>, std::shared_ptr<Ball>>> m_collided_balls;

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
		m_window = std::make_unique<RenderWindow>(
			VideoMode(wind_width, wind_height),
			"SFML works!", Style::Default, settings);

		// перемещение окна
		m_window->setPosition({ 0,0 });
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
				m_context->m_balls->AddBall(std::make_shared<BlueBall>(m_mouse_pos));
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
				m_shooting_vec = -(m_mouse_pos - (*m_moving_ball)->getPosition());
				lg.Info("shoot the ball by: " + std::to_string(m_shooting_vec.x) + " " + std::to_string(m_shooting_vec.y));

				// установка скорости для шара
				(*m_moving_ball)->SetVelocity(4.f * m_shooting_vec);

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
	// очистка списка пересекшихся вершин
	m_context->m_collided_balls.clear();

	/////////////////////////////////////
	//		СТАТИЧЕСКАЯ КОЛЛИЗИЯ
	/////////////////////////////////////

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
					// сохраняем пересекшиеся шары
					m_context->m_collided_balls.emplace_back(std::make_pair(*source, *target));

					// МЫ ВЫТЕСНЯЕМ ОДИН ШАР ИЗ ДРУГОГО

					// сохраним расстояние между центрами шаров по разным осям
					float x_dist = (*source)->GetXDist(**target);
					float y_dist = (*source)->GetYDist(**target);

					// Для начала следует найти расстояние между центром шаров
					float center_dist = (*source)->GetCenterDist(**target);

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

	/////////////////////////////////////
	//		ДИНАМИЧЕСКАЯ КОЛЛИЗИЯ
	/////////////////////////////////////

	// проходимся по всем парам пересекшихся шаров
	for (auto& collide_pair : m_context->m_collided_balls)
	{
		// первый шар из пары
		auto& fst_ball = collide_pair.first;
		// творой шар из пары
		auto& snd_ball = collide_pair.second;

		// пересчитываем растояние между центрами двух шаров
		float center_dist = fst_ball->GetCenterDist(*snd_ball);

		// считаем нормаль параллельную прямой,
		// проходящей через центры шаров
		Vector2f normal =
		{
			-fst_ball->GetXDist(*snd_ball) / center_dist,
			-fst_ball->GetYDist(*snd_ball) / center_dist
		};

		//lg.Info("normal: " + std::to_string(normal.x) + " " + std::to_string(normal.y));

		// отрисовка нормали
		/*{			
			Vertex line[] =
			{
				sf::Vertex(fst_ball->getPosition()),
				sf::Vertex(fst_ball->getPosition() + normal * center_dist)
			};
			line[0].color = Color::Red;
			line[1].color = Color::Red;

			m_context->m_window->draw(line, 2, sf::Lines);			
		}*/

		// строим перпендикулярный вектор нормали
		Vector2f tangent = { -normal.y, normal.x };

		// отрисовка перпендикуляра
		/*{
			Vertex line[] =
			{
				sf::Vertex(fst_ball->getPosition()),
				sf::Vertex(fst_ball->getPosition() + tangent * center_dist)
			};
			line[0].color = Color::Yellow;
			line[1].color = Color::Yellow;

			m_context->m_window->draw(line, 2, sf::Lines);
		}*/

		// скалярное произведение перпендикуляров
		auto vec1 = DotProduct(fst_ball->GetVelocity(), tangent);
		auto vec2 = DotProduct(snd_ball->GetVelocity(), tangent);
		Vector2f dpTan = { vec1.x + vec1.y, vec2.x + vec2.y };

		// скалярное произведение нормалей
		vec1 = DotProduct(fst_ball->GetVelocity(), normal);
		vec2 = DotProduct(snd_ball->GetVelocity(), normal);
		Vector2f dpNorm = { vec1.x + vec1.y, vec2.x + vec2.y };

		// закон сохранения импульса
		float m1 = 
			(dpNorm.x * (fst_ball->GetMass() - snd_ball->GetMass()) 
				+ 1.f * snd_ball->GetMass() * dpNorm.y) /
			(fst_ball->GetMass() + snd_ball->GetMass());
		float m2 =
			(dpNorm.y * (snd_ball->GetMass() - fst_ball->GetMass())
				+ 1.f * fst_ball->GetMass() * dpNorm.x) /
			(fst_ball->GetMass() + snd_ball->GetMass());

		// установка соответствующей скорости
		fst_ball->SetVelocity(tangent * (dpTan.x) + normal * m1);
		snd_ball->SetVelocity(tangent * (dpTan.y) + normal * m2);
	}

	/////////////////////////////////////
	//	  ОБНОВЛЕНИЕ ПОЛОЖЕНИЯ ШАРОВ
	/////////////////////////////////////

	for (auto& ball : m_context->m_balls->GetBalls())
	{
		// симуляция трат энергии путем создания отрицательного ускорения
		ball->SetAcceleration(-ball->GetVelocity() * 0.8f);

		// для каждого шара расчитываем новое положение
		// расчет скорости
		ball->IncreaseVelocity(ball->GetAcceleration() * m_context->m_dT);

		// расчет позиции
		ball->move(ball->GetVelocity() * m_context->m_dT);

		// проверка на выход за экран

		// выход за левую границу
		if (ball->getPosition().x - ball->getRadius() < 0)
		{
			ball->setPosition(Vector2f(ball->getRadius() + 1, ball->getPosition().y));
			ball->SetVelocity(Vector2f(-ball->GetVelocity().x, ball->GetVelocity().y));
		}
		// выход за правую границу
		else if (ball->getPosition().x > wind_width - ball->getRadius())
		{
			ball->setPosition(Vector2f(wind_width - ball->getRadius() - 1, ball->getPosition().y));
			ball->SetVelocity(Vector2f(-ball->GetVelocity().x, ball->GetVelocity().y));
		}
		// выход за верхнюю границу
		else if (ball->getPosition().y - ball->getRadius() < 0)
		{
			ball->setPosition(Vector2f(ball->getPosition().x, ball->getRadius() + 1));
			ball->SetVelocity(Vector2f(ball->GetVelocity().x, -ball->GetVelocity().y));
		}
		// выход за нижнюю границу
		else if (ball->getPosition().y > wind_height - ball->getRadius())
		{
			ball->setPosition(Vector2f(ball->getPosition().x, wind_height - ball->getRadius() - 1));
			ball->SetVelocity(Vector2f(ball->GetVelocity().x, -ball->GetVelocity().y));
		}

		// когда скорость очень маленькая, шары стоит остановить
		if (ball->GetVelocityScal() != 0 && ball->GetVelocityScal() < 0.0005f)
			ball->SetVelocity({ .0,.0 });
	}

	return false;
}

// главная функция
int main()
{
	// TODO: [] посмотреть второй урок, в котором реализуется коллизия шаров с плоскостями и сделать тоже самое
	//		 [+] Коллизия неправильно определяется с объектами разного радиуса (возможно проблема в скорости обработки физики, это тоже в уроке втором фиксится)

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
		std::make_shared<PhysicHandler>(cnxt)
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
		cnxt->m_window->draw(*cnxt->m_balls);
		
		// отрисовка линий контакта шаров
		for (auto& col_pair : cnxt->m_collided_balls)
		{
			Vertex line[] =
			{
				sf::Vertex(col_pair.first->getPosition()),
				sf::Vertex(col_pair.second->getPosition())
			};
			line[0].color = Color::Red;
			line[1].color = Color::Red;

			cnxt->m_window->draw(line, 2, sf::Lines);
		}

		cnxt->m_window->display();

	}

	return 0;
}

