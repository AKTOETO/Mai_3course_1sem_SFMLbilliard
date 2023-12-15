#include "BallPhysManager.h"

template<class From, class To>
inline std::vector<std::shared_ptr<To>> CastShPtrVectorDown(const std::vector<std::shared_ptr<From>>& vec)
{
	std::vector<std::shared_ptr<From>> out;
	out.reserve(vec.size());
	for (std::shared_ptr<From> el : vec)
	{
		out.push_back(std::dynamic_pointer_cast<std::shared_ptr<To>::spDerived>(el));
	}
}

Engine::BallPhysManager::BallPhysManager(
	std::shared_ptr<Engine::Context> _cntxt,
	const std::vector<std::shared_ptr<Ball>>& objects)
	:DynamicObjectManager(_cntxt, CastShPtrVectorDown<Ball, DynamicObject>(objects))
{
}

Engine::BallPhysManager::Borders Engine::BallPhysManager::IsOutOfBounds(
	std::shared_ptr<DynamicObject> obj)
{
	if (obj->GetPos().x < 0) return Borders::Left;
	if (obj->GetPos().x > m_context->m_win_size.x) return Borders::Right;
	if (obj->GetPos().y < 0) return Borders::Top;
	if (obj->GetPos().y > m_context->m_win_size.y) return Borders::Down;
	return Borders::None;
}

bool Engine::BallPhysManager::IsColliding(
	std::shared_ptr<DynamicObject> obj1,
	std::shared_ptr<DynamicObject> obj2)
{
	// вычисление оси столкновения (отрезок между двумя центрами шаров)
	// https://www.youtube.com/watch?v=lS_qeBy3aQI (4:09)
}

void Engine::BallPhysManager::ResolveCollision(
	std::shared_ptr<DynamicObject> obj1,
	std::shared_ptr<DynamicObject> obj2)
{
}
