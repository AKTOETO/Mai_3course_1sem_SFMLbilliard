#ifndef BALLOBJECTDOWNLOADMANAGER_H
#define BALLOBJECTDOWNLOADMANAGER_H

#include "SpriteDownloadManger.h"
#include "../objects/Ball.h"

namespace Engine
{
	// TODO: создать базовай загрузчик объектов, чтобы ID передавать в конструкторе.
	// загрузчик игрового объекта Ball
	class BallObjectDownloadManager :
		public IDownloadManager<Ball>
	{
	private:
		// загрузчик спрайтов
		std::unique_ptr<SpriteDownloadManager> m_sprite_download;

	public:
		BallObjectDownloadManager(std::shared_ptr<Engine::Context> _cntxt) :
			IDownloadManager(_cntxt),
			m_sprite_download(std::make_unique<SpriteDownloadManager>(_cntxt))
		{}

		// загрузка объекта Ball
		std::shared_ptr<Ball> Download(
			const std::string& _filepath
		) override;

	};
}

#endif // !BALLOBJECTDOWNLOADMANAGER_H

