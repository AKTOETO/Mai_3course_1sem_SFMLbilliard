#ifndef TEXTUREDOWNLOADMANAGER_H
#define TEXTUREDOWNLOADMANAGER_H

#include "IDownloadManager.h"
namespace Engine
{
	class TextureDownloadManager :
		public IDownloadManager<Texture>
	{
	public:
		TextureDownloadManager(std::shared_ptr<Engine::Context> _cntxt) :
			IDownloadManager(_cntxt)
		{}

		// загрузка текстуры
		std::shared_ptr<Texture> Download(const std::string& _filepath) override;
	};
}

#endif // !TEXTUREDOWNLOADMANAGER_H

