#include "pch.h"
#include "logger/Logger.h"

using namespace sf;

static int wind_width = 800;//500;
static int wind_height = 600;//100;

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

};

Ball::Ball(int id, float radius, Vector2f pos,
	Vector2f vel, Vector2f acc)
	:CircleShape(radius), m_id(id), m_vel(vel), m_acc(acc)
{
	setPosition(pos);
	setOutlineColor(Color::Black);
	setOutlineThickness(5);
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
		m_balls.emplace_back(std::move(ball));
	}

	// получить список шаров
	const std::list<std::shared_ptr<Ball>> GetBalls() const { return m_balls; }

	// нарисовать все шары
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_balls.empty())
			// рисуем каждый шар
			for (auto& ball : m_balls)
				target.draw(*ball, states);
	}
};

// класс кнопки (trigger)
class Button
{
public:

	// указатель на функцию
	using pfunc = std::function<void()>;

	// перечисление состояний
	enum class State
	{
		NONE = 0,
		PRESSED,	// нажата
		RELEASED,	// отпущена
		HOLDED		// нажата и не отпущена
	};

	// конструктор
	Button(Keyboard::Key key)
		:m_state(State::NONE), m_key_code(key),
		m_pressed_func(nullptr), m_released_func(nullptr),
		m_holded_func(nullptr)
	{}

	// обработка состояния кнопки на клавиатуре
	State Logic();

	// Установка новой кнопки
	void SetKeyCode(Keyboard::Key key) { m_key_code = key; }

	// Получение текущей кнопки
	Keyboard::Key GetKeyCode() const { return m_key_code; }

	// Установка функций обработки состояний
	// нажатие
	void SetPressedFunc(pfunc func) { m_pressed_func = func; }

	// удержание
	void SetHoldedFunc(pfunc func) { m_holded_func = func; }

	// отпускание
	void SetReleasedFunc(pfunc func) { m_released_func = func; }

protected:

	// текущее состояние кнопки
	State m_state;

	// клавиша, на которую срабатывает кнопка
	Keyboard::Key m_key_code;

	// функция, которая вызывается при нажатии
	pfunc m_pressed_func;

	// функция, которая вызывается при зажатии
	pfunc m_holded_func;

	// функция, которая вызывается при отпускании
	pfunc m_released_func;
};

Button::State Button::Logic()
{
	// если кнопка нажата
	if (Keyboard::isKeyPressed(m_key_code)
		&& m_state == State::NONE)
	{
		m_state = State::PRESSED;
		//lg.Info("pressed");
		if (m_pressed_func)m_pressed_func();
	}
	// если кнопка зажата
	else if (Keyboard::isKeyPressed(m_key_code) && m_state == State::PRESSED)
	{
		m_state = State::HOLDED;
		//lg.Info("holded");
		if (m_holded_func)m_holded_func();
	}
	// если кнопка отжата
	else if (!Keyboard::isKeyPressed(m_key_code) &&
		(
			m_state == State::PRESSED ||
			m_state == State::HOLDED
		))
	{
		m_state = State::RELEASED;
		//lg.Info("released");
		if (m_released_func)m_released_func();
	}
	// если кнопка уже давно отжата
	else if (!Keyboard::isKeyPressed(m_key_code) && m_state == State::RELEASED)
	{
		//lg.Info("none");
		m_state = State::NONE;
	}

	return m_state;
}

// главная функция
int main()
{
	// TODO: шары на нажатие лкм по пустой области появляются,
	// на нажатие и зажатие лкм по шару можно его бросить,
	// на нажатие пкм по шару можно его удалить
	// на нажатие колесика мышки шар можно перенести

	lg.Info("Start Program");

	srand(time(NULL));

	// настройки окна
	sf::ContextSettings settings;
	settings.depthBits = 4;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	// создание окна
	RenderWindow window(VideoMode(wind_width, wind_height), "SFML works!", Style::Default, settings);
	window.setPosition({ 0,0 });
	window.setFramerateLimit(60);

	// для расчета фпс и времени кадра
	sf::Clock clock;
	Time last_time = clock.getElapsedTime();
	Time cur_time;
	float fps;
	float dT;

	// Список шаров
	BallsVector balls;

	// создание кнопки добавления шаров
	Button add_ball(Keyboard::Key::A);

	// цикл программы
	while (window.isOpen())
	{
		cur_time = clock.getElapsedTime();
		dT = cur_time.asMicroseconds();
		dT /= 800;

		fps = 1.f / (cur_time.asSeconds() - last_time.asSeconds());
		last_time = cur_time;
		window.setTitle(
			L" fps: " + std::to_wstring(floor(fps))
		);

		Event evnt;

		// обработка событий
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
				// если нажата кнопка
			case Event::KeyPressed:
				// если нажат esc - выходим из программы
				if (evnt.key.code == Keyboard::Key::Escape)
				{
					lg.Info("Hitted Esc - exiting the program");
					window.close();
				}
				break;

			case Event::Closed:
				lg.Info("Exiting the program");
				window.close();
				break;

			case Event::Resized:
				wind_width = evnt.size.width;
				wind_height = evnt.size.height;
				lg.Info("Window resized " + std::to_string(wind_width) + " " + std::to_string(wind_height));
				FloatRect visib_area(0, 0, wind_width, wind_height);
				window.setView(View(visib_area));
				break;
			}
		}
		// обработка логики		
		if (add_ball.Logic() == Button::State::PRESSED)
		{
			balls.AddBlueBallAtRandPos();
			lg.Info("Ball added");
		}

		// проверка столкновений с границами

		// отрисовка всего
		window.clear(Color::Cyan);

		window.draw(balls);

		window.display();
	}

	return 0;
}
