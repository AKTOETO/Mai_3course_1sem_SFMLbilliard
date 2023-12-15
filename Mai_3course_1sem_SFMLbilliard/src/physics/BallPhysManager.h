#ifndef BALLPHYSMANAGER_H
#define BALLPHYSMANAGER_H

#include "../pch.h"
#include "DynamicObjectManager.h"
#include "../objects/Ball.h"

namespace Engine
{
	// менеджер физики для шаров
	class BallPhysManager :
		public DynamicObjectManager
	{
	public:
		BallPhysManager(std::shared_ptr<Engine::Context> _cntxt,
			const std::vector<std::shared_ptr<Ball>>& objects);

		// проверка выхода за поле одного объекта
		virtual Borders IsOutOfBounds(std::shared_ptr<DynamicObject> obj) override;

		// проверка двух объектов на столкновение
		virtual bool IsColliding(
			std::shared_ptr<DynamicObject> obj1,
			std::shared_ptr<DynamicObject> obj2) override;

		// решение коллизии двух объектов
		virtual void ResolveCollision(
			std::shared_ptr<DynamicObject> obj1,
			std::shared_ptr<DynamicObject> obj2) override;

	};

}


#endif // !BALLPHYSMANAGER_H

