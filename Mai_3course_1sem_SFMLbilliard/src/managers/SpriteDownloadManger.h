#ifndef SPRITEDOWNLOADMANAGER_H
#define SPRITEDOWNLOADMANAGER_H

#include "TextureDownloadManager.h"

namespace Engine
{
	// структрура спрайта с текстурой
	struct NewSprite
	{
		std::shared_ptr<Sprite> m_sprite;
		std::shared_ptr<Texture> m_texture;

		NewSprite() :
			m_sprite(new Sprite), m_texture(nullptr)
		{}

		void SetTexture(std::shared_ptr<Texture> _texture)
		{
			m_texture = _texture;
			if(_texture)
				m_sprite->setTexture(*m_texture);
		}

		std::shared_ptr<Sprite> operator()() { return m_sprite; }
	};

	// загрузчик спрайтов
	class SpriteDownloadManager :
		public IDownloadManager<NewSprite>
	{
	protected:
		std::unique_ptr<TextureDownloadManager> m_texture_downloader;

	public:
		SpriteDownloadManager(std::shared_ptr<Engine::Context> _cntxt) :
			IDownloadManager(_cntxt),
			m_texture_downloader(std::make_unique<TextureDownloadManager>(_cntxt))
		{}

		// загрузка спрайта
		std::shared_ptr<NewSprite> Download(
			const std::string& _filepath
		) override;

		// загрузка спрайта из готовой текстуры
		std::shared_ptr<NewSprite> Download(
			const std::string& _filepath,
			std::shared_ptr<Texture> _texture
		);
	};
}

#endif // !SPRITEDOWNLOADMANAGER_H
