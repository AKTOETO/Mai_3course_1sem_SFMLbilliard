#include "../pch.h"
#ifndef BALLOBJECTDOWNLOADMANAGER_H
#define BALLOBJECTDOWNLOADMANAGER_H

#include "SpriteDownloadManger.h"
#include "BaseObjectDownloadManager.h"
#include "../objects/Ball.h"

namespace Engine
{
	// загрузчик игрового объекта Ball
	class BallObjectDownloadManager :
		public BaseObjectDownloadManager<Ball>
	{
	private:
		// загрузчик спрайтов
		std::unique_ptr<SpriteDownloadManager> m_sprite_download;

	public:
		BallObjectDownloadManager(std::shared_ptr<Engine::Context> _cntxt) :
			BaseObjectDownloadManager(_cntxt),
			m_sprite_download(std::make_unique<SpriteDownloadManager>(_cntxt))
		{}

		// загрузка объекта Ball
		std::shared_ptr<Ball> Download(
			const std::string& _filepath = "assets/gameObjects/OneSpriteMoveableObject/ball.txt"
		) override;

	};
}

#endif // !BALLOBJECTDOWNLOADMANAGER_H

