#include "pch.h"
#include "logger/Logger.h"

using namespace sf;

static int wind_width = 1200;//500;
static int wind_height = 1000;//100;

static Vector2i wind_center(wind_width / 2, wind_height / 2);

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
}

int main()
{
	// логгер
	Logger log(Logger::LogMode::ConsoleAndFile);
	log.Info("Start Program");


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

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::Resized:
				wind_width = event.size.width;
				wind_height = event.size.height;
				FloatRect visib_area(0, 0, wind_width, wind_height);
				window.setView(View(visib_area));
				break;
			}
		}

		// проверка столкновений с границами

		window.clear(Color::Cyan);


		window.display();
	}

	return 0;
}

