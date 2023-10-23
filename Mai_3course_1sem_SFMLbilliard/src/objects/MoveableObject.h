#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include "../pch.h"

// структура движимого объекта
struct MoveableObject
{
	// идентификатор объекта
	int id;

	// вектор позиции объекта
	Vector2f m_pos;

	// вектор скорости объекта
	Vector2f m_vel;

	// вектор ускорения объекта
	Vector2f m_acc;

	// масса объекта
	float m_mass;
};

#endif // !MOVEABLEOBJECT_H
