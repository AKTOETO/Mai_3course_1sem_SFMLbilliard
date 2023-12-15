#include "DynamicObjectManager.h"

Engine::DynamicObjectManager::DynamicObjectManager(
	std::shared_ptr<Engine::Context> _cntxt,
	const std::vector<std::shared_ptr<DynamicObject>>& objects
) :m_context(_cntxt), m_objects(objects)
{}

void Engine::DynamicObjectManager::ConstrainCheck()
{	// сравниваем каждый объект с каждым
	for (auto obj : m_objects)
	{
		// проверяем, вышел ли объект за границы
		switch (IsOutOfBounds(obj))
		{
			// вычисление скорости
			Vector2f vel = obj->GetPos() - obj->GetOldPos();

			// разрешение коллизии
		case Borders::Left:
			OutOfLeftBorder(obj, vel);
			break;
		case Borders::Right:
			OutOfRightBorder(obj, vel);
			break;
		case Borders::Top:
			OutOfTopBorder(obj, vel);
			break;
		case Borders::Down:
			OutOfDownBorder(obj, vel);
			break;
		}
	}
}

void Engine::DynamicObjectManager::OutOfLeftBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel)
{
	obj->SetPos({ 0, obj->GetPos().y });
	obj->SetOldPos({ obj->GetPos().x + vel.x, obj->GetOldPos().y });
}

void Engine::DynamicObjectManager::OutOfRightBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel)
{
	obj->SetPos({ m_context->m_win_size.x,obj->GetPos().y });
	obj->SetOldPos({ obj->GetPos().x + vel.x, obj->GetOldPos().y });
}

void Engine::DynamicObjectManager::OutOfTopBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel)
{
	obj->SetPos({ obj->GetPos().x, 0 });
	obj->SetOldPos({ obj->GetOldPos().x, obj->GetPos().y + vel.y });
}

void Engine::DynamicObjectManager::OutOfDownBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel)
{
	obj->SetPos({ obj->GetPos().x, m_context->m_win_size.y });
	obj->SetOldPos({ obj->GetOldPos().x, obj->GetPos().y + vel.y });
}

void Engine::DynamicObjectManager::CollideCheck(float dT)
{
	//TODO по два раза коллизия проверяется
	// надо чтобы внутренний цикл шел от obj1+1 объекта
	
	// сравниваем каждый объект с каждым
	for (auto obj1 : m_objects)
	{
		for (auto obj2 : m_objects)
		{
			// если obj1 И obj2 - разные объекты, проверяем их на столкновение
			if (obj1 != obj2)
			{
				if (IsColliding(obj1, obj2)) ResolveCollision(obj1, obj2);
			}
		}
	}
}

void Engine::DynamicObjectManager::Update(float dT)
{
	// проверка выхода за доступную область
	ConstrainCheck();

	// проверка столкновения объектов
	CollideCheck(dT);

	// обновление позиции всех объектов
	for (auto& el : m_objects)el->Update(dT);
}
