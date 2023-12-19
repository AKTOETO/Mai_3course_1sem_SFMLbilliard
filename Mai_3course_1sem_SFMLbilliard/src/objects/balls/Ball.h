#ifndef BALL_H
#define BALL_H

#include "../../pch.h"
#include "../../math/Vec2.h"

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

	// оставшееся время симуляции физики
	float m_remanin_sim_time;

	// старая позиция мяча
	Vector2f m_old_pos;

public:
	Ball(float radius = 10, Vector2f pos = { 0,0 },
		Vector2f vel = { 0,0 }, Vector2f acc = { 0,0 });
	~Ball() {};

	// Get/Set методы
	const Vector2f& GetVelocity() const { return m_vel; }
	const float& GetVelocityScal() const { return sqrt(m_vel.x * m_vel.x + m_vel.y * m_vel.y); }
	const Vector2f& GetAcceleration() const { return m_acc; }
	const float& GetMass() const { return m_mass; }
	const float& GetSimTime() const { return m_remanin_sim_time; }
	Vector2f GetOldPos() const { return m_old_pos; }

	void SetVelocity(const Vector2f& vel) { m_vel = vel; }
	void SetAcceleration(const Vector2f& acc) { m_acc = acc; }
	void SetSimTime(const float& time) { m_remanin_sim_time = time; }
	void SetOldPos(const Vector2f& vec) { m_old_pos = vec; }

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


#endif
