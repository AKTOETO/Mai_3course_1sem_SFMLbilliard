#include "pch.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main()
{
	RenderWindow window(VideoMode(400, 400), L"Бильярд");

	window.setVerticalSyncEnabled(true);

	CircleShape shape(100.f, 64);
	shape.setPosition(100, 100);
	shape.setFillColor(Color::Magenta);
	shape.setRotation(45);

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}

		window.clear(Color::Blue);
		window.draw(shape);
		window.display();
	}
}