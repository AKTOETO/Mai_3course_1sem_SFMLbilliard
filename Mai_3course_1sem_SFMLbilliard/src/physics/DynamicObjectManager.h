#ifndef DYNAMICOBJECTMANAGER_H
#define DYNAMICOBJECTMANAGER_H
#include "../pch.h"

#include "../Context.h"
#include "DynamicObject.h"

namespace Engine
{
	// управляющий списком объектов 
	class DynamicObjectManager
	{
	protected:
		// список объектов
		std::vector<std::shared_ptr<DynamicObject>> m_objects;

		// контекст системы
		std::shared_ptr<Engine::Context> m_context;

		// границы
		enum class Borders
		{
			None = 0,
			Left,
			Right,
			Top,
			Down
		};

	public:
		DynamicObjectManager(std::shared_ptr<Engine::Context> _cntxt,
			const std::vector<std::shared_ptr<DynamicObject>>& objects);
		virtual ~DynamicObjectManager() {};

		// проверка выхода за поле
		virtual void ConstrainCheck();

		// проверка выхода за поле одного объекта
		virtual Borders IsOutOfBounds(std::shared_ptr<DynamicObject> obj) = 0;

		// решение выхода за границы
		// левая граница
		virtual void OutOfLeftBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel);
		// правая граница
		virtual void OutOfRightBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel);
		// нижняя граница
		virtual void OutOfDownBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel);
		// верхняя граница
		virtual void OutOfTopBorder(std::shared_ptr<DynamicObject> obj, Vector2f& vel);

		// проверка двух объектов на столкновение
		virtual bool IsColliding(std::shared_ptr<DynamicObject> obj1, std::shared_ptr<DynamicObject> obj2) = 0;

		// решение коллизии двух объектов
		virtual void ResolveCollision(std::shared_ptr<DynamicObject> obj1, std::shared_ptr<DynamicObject> obj2) = 0;

		// проверка столкновений объектов друг с другом
		virtual void CollideCheck(float dT);

		// обновление физики
		virtual void Update(float dT);
	};
}

#endif // !DYNAMICOBJECTMANAGER_H