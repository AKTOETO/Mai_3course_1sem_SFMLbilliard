#ifndef WORLD_H
#define WORLD_H

#include "../managers/BallObjectDownloadManager.h"
#include "../physics/BallsCollision.h"

// TODO: Должен быть класс загрузчика мира, который будет наследоваться от 

// класс мира, в котором находятся все игровые объекты
// Он является контейнером с данными для различных менеджеров
// Таких как менеджер физики(далее будет создан), 
// менеджер контроллируемых объектов, ... (мб ничего больше и не будет :( )
namespace Engine
{
	class World//:
		//public BallsCollision
	{
	protected:
		// массив шаров на игровом столе
		std::vector<std::shared_ptr<Ball>> m_balls;

		// игровой стол

		// кий (управляемый игроком)

	public:
		World(std::shared_ptr<Engine::Context> _cntxt);
		
	};
}

#endif // !WORLD_H

