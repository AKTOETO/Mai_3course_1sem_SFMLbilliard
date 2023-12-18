#ifndef VEC2_H
#define VEC2_H

#include "../pch.h"

// нормализация вектора
inline Vector2f NormalizeVector(Vector2f _vect)
{
	float norm = sqrt(_vect.x * _vect.x + _vect.y * _vect.y);
	return _vect / norm;
}

// скалярное произведение двух векторов
inline Vector2f DotProduct(Vector2f vec1, Vector2f vec2)
{
	return { vec1.x * vec2.x, vec1.y * vec2.y };
}

// расстояние между точками
inline float DistBetweenPoints(const Vector2f& vec, const Vector2f& vec2)
{
	return sqrtf(powf(vec.x - vec2.x, 2) + powf(vec.y - vec2.y, 2));
}

// получение случайной точки на экране
inline Vector2i GetWindowRandPoint(sf::RenderWindow& window)
{
	return Vector2i(rand() % window.getSize().x, rand() % window.getSize().y);
}

#endif
