#ifndef IDOWNLOADMANAGER_H
#define IDOWNLOADMANAGER_H

#include "BaseManager.h"

namespace Engine
{
	template<class Ty>
	class IDownloadManager:
		public BaseManager
	{
	public:
		IDownloadManager(std::shared_ptr<Engine::Context> _cntxt) :
			BaseManager(_cntxt)
		{}
		//virtual ~IDownloadManager() = 0;

		// метод загрузки объекта
		virtual std::shared_ptr<Ty> Download(const std::string& _filename) = 0;
	};
}

#endif // !IDOWNLOADMANAGER_H
