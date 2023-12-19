#include "Ball.h"

Ball::Ball(float radius, Vector2f pos,
	Vector2f vel, Vector2f acc)
	:CircleShape(radius), m_vel(vel), m_acc(acc),
	m_outline_thickness(5), m_remanin_sim_time(0),
	m_old_pos({ .0,.0 })
{
	// радиус учитывает толщину обводки
	m_radius = (radius + m_outline_thickness);
	// масса зависит от радиуса
	m_mass = m_radius * 5.f;

	setPosition(pos);
	setOutlineColor(Color::Black);
	setOutlineThickness(m_outline_thickness);
	setOrigin({ radius, radius });
}

bool Ball::IsIntersected(const Ball& ball)
{
	// расстоние между центрами
	float center_dist = GetCenterDist(ball);

	// если расстояние между центрами равно сумме радиусов,
	// то шары пересеклись
	return fabs(center_dist) <= this->getRadius() + ball.getRadius();
}

float Ball::GetCenterDist(const Ball& ball)
{
	// Для начала следует найти расстояние между центром шаров
	return sqrtf(powf(GetXDist(ball), 2) + powf(GetYDist(ball), 2));
}

float Ball::GetXDist(const Ball& ball)
{
	return (this->getPosition().x - ball.getPosition().x);
}

float Ball::GetYDist(const Ball& ball)
{
	return (this->getPosition().y - ball.getPosition().y);
}
