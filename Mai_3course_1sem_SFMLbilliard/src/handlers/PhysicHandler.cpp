#include "PhysicHandler.h"

PhysicHandler::PhysicHandler(std::shared_ptr<ProgContext> context)
	:ABCHandler(context), m_sim_updates(1), m_sim_elapsed_time(.0),
	m_max_sim_steps(2)
{}

bool PhysicHandler::LogicHandling()
{
	// очистка списка пересекшихся вершин
	m_context->m_collided_balls.clear();

	// расчет времени симуляции каждого подшага
	m_sim_elapsed_time = m_context->m_dT / m_sim_updates;

	// главный цикл симуляции
	for (int i = 0; i < m_sim_updates; i++)
	{
		// установка времени симуляции для 
		// каждого мяча на максимально возможное

		for (auto& ball : m_context->m_balls->GetBalls())
			ball->SetSimTime(m_sim_elapsed_time);

		/////////////////////////////////////
		//	  ОБНОВЛЕНИЕ ПОЛОЖЕНИЯ ШАРОВ
		/////////////////////////////////////

		for (int i = 0; i < m_max_sim_steps; i++)
		{
			for (auto& ball : m_context->m_balls->GetBalls())
			{
				// если у мяча еще осталось время на моделирование физики
				if (ball->GetSimTime() > 0.0f)
				{
					// обновление старого положения мяча
					ball->SetOldPos(ball->getPosition());

					// симуляция трат энергии путем создания отрицательного ускорения
					ball->SetAcceleration(-ball->GetVelocity() * 0.8f);

					// ДЛЯ КАЖДОГО ШАРА РАСЧИТЫВАЕМ НОВОЕ ПОЛОЖЕНИЕ
					// расчет скорости с учетем оставшегося времени симуляции
					// для текущего мяча
					ball->IncreaseVelocity(ball->GetAcceleration() * ball->GetSimTime());

					// расчет позиции
					ball->move(ball->GetVelocity() * ball->GetSimTime());

					// проверка на выход за экран

					// выход за левую границу
					if (ball->getPosition().x - ball->getRadius() < 0)
					{
						ball->setPosition(Vector2f(ball->getRadius() + 1, ball->getPosition().y));
						ball->SetVelocity(Vector2f(-ball->GetVelocity().x, ball->GetVelocity().y));
					}
					// выход за правую границу
					else if (ball->getPosition().x > m_context->wind_width - ball->getRadius())
					{
						ball->setPosition(Vector2f(m_context->wind_width - ball->getRadius() - 1, ball->getPosition().y));
						ball->SetVelocity(Vector2f(-ball->GetVelocity().x, ball->GetVelocity().y));
					}
					// выход за верхнюю границу
					else if (ball->getPosition().y - ball->getRadius() < 0)
					{
						ball->setPosition(Vector2f(ball->getPosition().x, ball->getRadius() + 1));
						ball->SetVelocity(Vector2f(ball->GetVelocity().x, -ball->GetVelocity().y));
					}
					// выход за нижнюю границу
					else if (ball->getPosition().y > m_context->wind_height - ball->getRadius())
					{
						ball->setPosition(Vector2f(ball->getPosition().x, m_context->wind_height - ball->getRadius() - 1));
						ball->SetVelocity(Vector2f(ball->GetVelocity().x, -ball->GetVelocity().y));
					}

					// когда скорость очень маленькая, шары стоит остановить
					if (ball->GetVelocityScal() != 0 && ball->GetVelocityScal() < 0.05f)
						ball->SetVelocity({ .0,.0 });
				}
			}
		}

		/////////////////////////////////////
		//		СТАТИЧЕСКАЯ КОЛЛИЗИЯ
		/////////////////////////////////////

		// итератор для перебора первого шара
		std::list<std::shared_ptr<Ball>>::const_iterator source = m_context->m_balls->GetBalls().begin();

		// проходимся по всем шарам и пытаемся пересечь их друг с другом
		while (source != m_context->m_balls->GetBalls().end())
		{
			// итератор для перебора второго шара
			std::list<std::shared_ptr<Ball>>::const_iterator target = m_context->m_balls->GetBalls().begin();

			// проходимся по всем шарам и пытаемся пересечь их друг с другом
			while (target != m_context->m_balls->GetBalls().end())
			{
				// если это разные шары
				if (source != target)
				{
					// проверяем их на пересечение
					if ((*source)->IsIntersected(**target))
					{
						// сохраняем пересекшиеся шары
						m_context->m_collided_balls.emplace_back(std::make_pair(*source, *target));

						// МЫ ВЫТЕСНЯЕМ ОДИН ШАР ИЗ ДРУГОГО

						// сохраним расстояние между центрами шаров по разным осям
						float x_dist = (*source)->GetXDist(**target);
						float y_dist = (*source)->GetYDist(**target);

						// Для начала следует найти расстояние между центром шаров
						float center_dist = (*source)->GetCenterDist(**target);

						// расстояние, на которое они пересекаются
						// на это расстояние мы сдвинем каждый шар, поэтому делим его на 2
						float overlap_dist = 0.5 * (center_dist - (*source)->getRadius() - (*target)->getRadius());

						// сдвигаем шары друг относительно друга

						// если шаром не управляют мышкой, то двигаем его
						if (source != m_context->m_following_ball)
							(*source)->move(Vector2f(
								-overlap_dist * (x_dist) / center_dist,
								-overlap_dist * (y_dist) / center_dist
							));

						// если шаром не управляют мышкой, то двигаем его
						if (target != m_context->m_following_ball)
							(*target)->move(Vector2f(
								overlap_dist * (x_dist) / center_dist,
								overlap_dist * (y_dist) / center_dist
							));

					}
				}
				target++;
			}

			// РАСЧЕТ ОСТАВШЕГОСЯ ВРЕМЕНИ ШАРА
			// С УЧЕТОМ ТОГО, ЧТО ОН СТОЛКНУЛСЯ С КЕМ-ТО

			// определение предполагаемой скорости шара
			float sup_speed = (*source)->GetVelocityScal();

			// определение предполагаемого пройденного расстояния шаром
			float sup_distance = sup_speed * (*source)->GetSimTime();

			// настоящее пройденное расстояние шаром
			// (из-за столкновения оно может быть другим)
			float actual_dist = DistBetweenPoints((*source)->getPosition(), (*source)->GetOldPos());

			// фактически затраченное время
			float actual_time = actual_dist / sup_speed;

			// изменяем оставшееся время моделирования физики шара
			(*source)->SetSimTime((*source)->GetSimTime() - actual_time);

			source++;
		}

		/////////////////////////////////////
		//		ДИНАМИЧЕСКАЯ КОЛЛИЗИЯ
		/////////////////////////////////////

		// проходимся по всем парам пересекшихся шаров
		for (auto& collide_pair : m_context->m_collided_balls)
		{
			// первый шар из пары
			auto& fball = collide_pair.first;
			// творой шар из пары
			auto& sball = collide_pair.second;

			// пересчитываем растояние между центрами двух шаров
			float center_dist = fball->GetCenterDist(*sball);

			// считаем нормаль параллельную прямой,
			// проходящей через центры шаров
			Vector2f normal =
			{
				-fball->GetXDist(*sball) / center_dist,
				-fball->GetYDist(*sball) / center_dist
			};

			// строим перпендикулярный вектор нормали
			Vector2f tangent = { -normal.y, normal.x };

			// скалярное произведение перпендикуляров
			auto vec1 = DotProduct(fball->GetVelocity(), tangent);
			auto vec2 = DotProduct(sball->GetVelocity(), tangent);
			Vector2f dpTan = { vec1.x + vec1.y, vec2.x + vec2.y };

			// скалярное произведение нормалей
			vec1 = DotProduct(fball->GetVelocity(), normal);
			vec2 = DotProduct(sball->GetVelocity(), normal);
			Vector2f dpNorm = { vec1.x + vec1.y, vec2.x + vec2.y };

			// ЗАКОН СОХРАНЕНИЯ ИМПУЛЬСА
			// скорость первого после удара
			float m1 =
				(dpNorm.x * (fball->GetMass() - sball->GetMass())
					+ 2.f * sball->GetMass() * dpNorm.y) /
				(fball->GetMass() + sball->GetMass());

			// скорость второго после удара
			float m2 =
				(dpNorm.y * (sball->GetMass() - fball->GetMass())
					+ 2.f * fball->GetMass() * dpNorm.x) /
				(fball->GetMass() + sball->GetMass());

			// установка соответствующей скорости
			fball->SetVelocity(tangent * (dpTan.x) + normal * m1);
			sball->SetVelocity(tangent * (dpTan.y) + normal * m2);
		}
	}
	return false;
}
