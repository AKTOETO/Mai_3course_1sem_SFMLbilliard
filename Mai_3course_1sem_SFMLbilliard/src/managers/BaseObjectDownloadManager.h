#ifndef BASEOBJECTDOWNLOADMANAGER_H
#define BASEOBJECTDOWNLOADMANAGER_H

#include "IDownloadManager.h"

namespace Engine
{
	// базовый загрузчик объектов 
	template<class Ty>
	class BaseObjectDownloadManager:
		public IDownloadManager<Ty>
	{
	public:
		BaseObjectDownloadManager(std::shared_ptr<Engine::Context> _cntxt):
			IDownloadManager<Ty>(_cntxt)
		{}

		// получить свободный идентификатор для нового объекта
		int GetFreeId() const { return BaseManager::m_context->free_id++; }
	};
}

#endif // !BASEOBJECTDOWNLOADMANAGER_H

