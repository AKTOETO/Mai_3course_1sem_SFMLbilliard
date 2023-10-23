#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include "../pch.h"

// ��������� ��������� �������
struct MoveableObject
{
	// ������������� �������
	int id;

	// ������ ������� �������
	Vector2f m_pos;

	// ������ �������� �������
	Vector2f m_vel;

	// ������ ��������� �������
	Vector2f m_acc;

	// ����� �������
	float m_mass;
};

#endif // !MOVEABLEOBJECT_H
