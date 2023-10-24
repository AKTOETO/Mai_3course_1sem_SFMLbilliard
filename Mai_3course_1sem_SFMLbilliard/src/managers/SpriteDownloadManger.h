#ifndef SPRITEDOWNLOADMANAGER_H
#define SPRITEDOWNLOADMANAGER_H

#include "TextureDownloadManager.h"
#include "../datatypes/CustomSprite.h"

namespace Engine
{
	// загрузчик спрайтов
	class SpriteDownloadManager :
		public IDownloadManager<CustomSprite>
	{
	protected:
		std::unique_ptr<TextureDownloadManager> m_texture_downloader;

	public:
		SpriteDownloadManager(std::shared_ptr<Engine::Context> _cntxt) :
			IDownloadManager(_cntxt),
			m_texture_downloader(std::make_unique<TextureDownloadManager>(_cntxt))
		{}

		// загрузка спрайта
		std::shared_ptr<CustomSprite> Download(
			const std::string& _filepath
		) override;

		// загрузка спрайта из готовой текстуры
		std::shared_ptr<CustomSprite> Download(
			const std::string& _filepath,
			std::shared_ptr<Texture> _texture
		);
	};
}

#endif // !SPRITEDOWNLOADMANAGER_H
