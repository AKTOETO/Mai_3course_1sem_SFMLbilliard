#ifndef DYNAMICOBJECT_H
#define DYNAMICOBJECT_H

#include "../pch.h"

namespace Engine
{
	// базовый класс динамического объекта
	class DynamicObject
	{
	protected:
		Vector2f m_position;		// текущая позиция
		Vector2f m_old_position;	// старая позиция
		float m_mass;				// масса
		int m_id;					// идентификатор объекта
		Vector2f m_force;			// сила, действующая на объект

	public:
		DynamicObject(int id, Vector2f pos = { 0.f,0.f }, float mass = 1);
		virtual ~DynamicObject() {};

		// обновление позиции объекта
		void Update(float dT);		

		// приложение силы к объекту
		void ApplyForce(const Vector2f& force);

		// сместиться на
		virtual void Move(Vector2f dist);

		// set / get методы
		virtual void SetMass(float mass);
		virtual void SetPos(Vector2f pos);
		virtual void SetOldPos(Vector2f pos);
		 
		virtual float GetMass() const;
		virtual Vector2f GetPos() const;
		virtual Vector2f GetOldPos() const;
		virtual Vector2f GetForce() const;
	};

}

#endif // !DYNAMICOBJECT_H
