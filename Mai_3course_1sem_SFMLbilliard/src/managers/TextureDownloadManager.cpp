#include "TextureDownloadManager.h"

namespace Engine
{
	std::shared_ptr<Texture> TextureDownloadManager::Download(const std::string& _filepath)
	{
		std::shared_ptr<Texture> texture(new Texture);
		if (!texture->loadFromFile(_filepath))
		{
			WARN("file: \'" + _filepath + "\' was not uploaded");
		}
		return texture;
	}
}
