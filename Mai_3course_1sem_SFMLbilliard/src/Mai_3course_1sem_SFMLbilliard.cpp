#include "pch.h"
#include "managers/SpriteDownloadManger.h"

using namespace sf;

static int wind_width = 1200;//500;
static int wind_height = 1000;//100;

static Vector2i wind_center(wind_width / 2, wind_height / 2);

static int count_of_cats = 20;

class Physics;
class Cat : public Drawable
{
	friend Physics;
protected:
	Sprite* m_sprite;
	Vector2f m_move_vector;
	float m_acceleration;
	float m_min_speed;
	float m_max_speed;
	float m_cur_speed;

public:
	Cat(Sprite* _sprite, Vector2f _dir) :
		m_sprite(_sprite), m_move_vector(_dir),
		m_min_speed(0.f), m_max_speed(10000), m_acceleration(0.05), m_cur_speed(100)
	{
		//std::cout << "pos: " << GetPos().x << " " << GetPos().y << "\n";
	}

	Vector2f GetPos() const { return m_sprite->getGlobalBounds().getPosition(); }
	Vector2f GetSize() const { return m_sprite->getGlobalBounds().getSize(); }

	FloatRect GetGlobBounds() const { return m_sprite->getGlobalBounds(); }

	Vector2f GetMoveVector() const { return m_move_vector; }
	void SetMoveVector(Vector2f _vect) { m_move_vector = _vect; }

	void ProcessLogic(float _dtime)
	{}

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(*m_sprite, states);
	}
};

Vector2f NormalizeVector(Vector2f _vect)
{
	float norm = sqrt(_vect.x * _vect.x + _vect.y * _vect.y);
	return _vect / norm;
}

Vector2f RandomVector()
{
	return Vector2f(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2);
}

Vector2f RandomVector(int width, int height)
{
	return Vector2f(rand() % width, rand() % height);
}

Sprite* CreateSprite(Texture& _texture)
{
	Sprite* sprite = new Sprite;
	sprite->setTexture(_texture);
	sprite->setTextureRect(IntRect(1, 1, 63, 63));
	//sprite->setOrigin(sf::Vector2f(25.f, 25.f));
	sprite->setPosition(RandomVector(wind_width, wind_height));
	return sprite;
}


class Physics
{
protected:
	std::vector<Cat>* m_cats;
	float m_energy_loss;		// потреи энергии (0; 1)

	std::vector<std::vector<bool>> was_collision;	// матрица коллизий произошедщих на прошлом шаге

public:
	Physics(std::vector<Cat>* _cats) :
		m_cats(_cats), m_energy_loss(0.1f),
		was_collision(count_of_cats, std::vector<bool>(count_of_cats, 0))
	{}

	void ProcessCollide(float _dtime)
	{
		// check collision with borders;
		ProcessCollideWithBorders(_dtime);

		// check collision cat with cat
		for (int i = 0; i < count_of_cats; i++)
		{
			for (int j = i + 1; j < count_of_cats; j++)
			{
				// if these cats are different AND
				// didnt collide
				if (i != j && !was_collision[i][j] &&
					(*m_cats)[i].GetGlobBounds().intersects((*m_cats)[j].GetGlobBounds()))
				{
					was_collision[i][j] = 1;
					auto temp = (*m_cats)[i].GetMoveVector() * (1 - m_energy_loss);
					(*m_cats)[i].SetMoveVector((*m_cats)[j].GetMoveVector() * (1 - m_energy_loss));
					(*m_cats)[j].SetMoveVector(temp);
				}
				else if (!(*m_cats)[i].GetGlobBounds().intersects((*m_cats)[j].GetGlobBounds()))
					was_collision[i][j] = 0;
			}
		}
		// move cats
		ProcessMoveCats(_dtime);
	}
private:
	void ProcessMoveCats(float _dtime)
	{
		for (Cat& cat : *m_cats)
		{
			// moving sprite
			cat.m_sprite->move(cat.m_move_vector * cat.m_cur_speed * (1 - m_energy_loss));

			if (cat.m_cur_speed > cat.m_min_speed) cat.m_cur_speed -= cat.m_acceleration;
			if (cat.m_cur_speed < cat.m_min_speed) cat.m_cur_speed = cat.m_min_speed;
			else if (cat.m_cur_speed > cat.m_max_speed)cat.m_cur_speed = cat.m_max_speed;
		}
	}

	void ProcessCollideWithBorders(float _dtime)
	{
		for (Cat& cat : *m_cats)
		{

			// bound checking 
			// right bound
			if (cat.m_sprite->getGlobalBounds().left + cat.m_sprite->getGlobalBounds().width >= wind_width &&
				cat.m_move_vector.x >= 0)
			{
				cat.m_move_vector *= (1 - m_energy_loss);
				cat.m_move_vector.x *= -1;
				//cat.m_sprite->setPosition(wind_width - cat.GetSize().x, cat.GetPos().y);
				//cat.m_cur_speed = cat.m_min_speed;
			}

			// left bound
			if (cat.m_sprite->getGlobalBounds().left <= 0 && cat.m_move_vector.x <= 0)
			{
				cat.m_move_vector *= (1 - m_energy_loss);
				cat.m_move_vector.x *= -1;
				//cat.m_sprite->setPosition(0, cat.GetPos().y);
				//cat.m_cur_speed = cat.m_min_speed;
			}

			// up bound
			if (cat.m_sprite->getGlobalBounds().top <= 0 && cat.m_move_vector.y <= 0)
			{
				cat.m_move_vector *= (1 - m_energy_loss);
				cat.m_move_vector.y *= -1;
				//cat.m_sprite->setPosition(cat.GetPos().x, 0);
				//cat.m_cur_speed = cat.m_min_speed;
			}

			// down bound
			if (cat.m_sprite->getGlobalBounds().top + cat.m_sprite->getGlobalBounds().height >= wind_height &&
				cat.m_move_vector.y >= 0)
			{
				cat.m_move_vector *= (1 - m_energy_loss);
				cat.m_move_vector.y *= -1;
				//cat.m_sprite->setPosition(cat.GetPos().x, wind_height - cat.GetSize().y);
				//cat.m_cur_speed = cat.m_min_speed;
			}
		}
	}
};

int main()
{
	srand(time(NULL));

	sf::ContextSettings settings;
	settings.depthBits = 4;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 16;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	RenderWindow window(VideoMode(wind_width, wind_height), "SFML works!", Style::Default, settings);
	window.setPosition({ 0,0 });
	window.setFramerateLimit(60);

	sf::Texture texture;
	if (!texture.loadFromFile("assets/textures/balls/Sprite-0001.png"))
		std::cout << "texture didnt load\n";

	std::vector<Cat> cats;


	cats.reserve(count_of_cats);
	for (int i = 0; i < count_of_cats; i++)
		cats.emplace_back(CreateSprite(texture), NormalizeVector(RandomVector()));

	Physics phys(&cats);

	sf::Clock clock;
	Time last_time = clock.getElapsedTime();
	Time cur_time;
	float fps;
	float dT;

	auto cnt = std::make_shared<Engine::Context>();

	//Engine::SpriteDownloadManager dwn(cnt);
	////auto sp = dwn.Download("assets/sprites/ball.txt");
	//Engine::TextureDownloadManager tdwn(cnt);
	//std::shared_ptr<Texture> text = tdwn.Download("kot.jpg");
	//auto sp = dwn.Download("assets/sprites/ball.txt");
	Engine::TextureDownloadManager tdwn(cnt);
	std::shared_ptr<Texture> text = tdwn.Download("kot.jpg");
	auto sp = dwn.Download("assets/sprites/ball.txt");

	while (window.isOpen())
	{
		cur_time = clock.getElapsedTime();
		dT = cur_time.asMicroseconds();
		dT /= 800;

		fps = 1.f / (cur_time.asSeconds() - last_time.asSeconds());
		last_time = cur_time;
		window.setTitle(
			L"cats: " + std::to_wstring(count_of_cats) +
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
		//phys.ProcessCollide(dT);


		window.clear(Color::Cyan);


		window.draw(*(*sp)());

		//for (Cat& cat : cats)window.draw(cat);

		window.display();
	}

	return 0;
}
