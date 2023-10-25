#ifndef MOVEABLEOBJECT_H
#define MOVEABLEOBJECT_H

#include "../pch.h"

namespace Engine
{
	// структура движимого объекта
	class MoveableObject
	{
	protected:
		// идентификатор объекта
		int m_id;

		// вектор позиции объекта
		Vector2f m_pos;

		// вектор скорости объекта
		Vector2f m_vel;

		// вектор ускорения объекта
		Vector2f m_acc;

		// масса объекта
		float m_mass;

	public:
		MoveableObject(
			int id, Vector2f pos = { 0.f,0.f },
			Vector2f vel = { 0.f,0.f },
			Vector2f acc = { 0.f,0.f },
			float mass = 0) :
			m_id(id), m_pos(pos), m_vel(vel), m_acc(acc), m_mass(mass)
		{}

		// get/set методы
		void SetId(int id) { m_id = id; }
		void SetPos(Vector2f pos) { m_pos = pos; }
		void SetVel(Vector2f vel) { m_vel = vel; }
		void SetAcc(Vector2f acc) { m_acc = acc; }
		void SetMass(float mass) { m_mass = mass; }

		int GetId() const { return m_id; }
		Vector2f GetPos() const { return m_pos; }
		Vector2f GetVel() const { return m_vel; }
		Vector2f GetAcc() const { return m_acc; }
		float GetMass() const { return m_mass; }

		// движение объекта
		void Move(Vector2f dist) { m_pos += dist; }
	};
}

#endif // !MOVEABLEOBJECT_H
