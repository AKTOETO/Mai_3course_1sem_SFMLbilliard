#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include "../Context.h"
namespace Engine
{
	// базовый класс менеджера
	class BaseManager
	{
	protected:
		std::shared_ptr<Engine::Context> m_context;

	public:
		BaseManager(std::shared_ptr<Engine::Context> _cntxt) :
			m_context(_cntxt)
		{}
	};
}

#endif