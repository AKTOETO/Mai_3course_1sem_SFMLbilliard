#include "pch.h"
#include "SpriteDownloadManger.h"

namespace Engine
{
	std::shared_ptr<NewSprite> Engine::SpriteDownloadManager::Download(const std::string& _filepath)
	{
		std::shared_ptr<NewSprite> sp(new NewSprite);

		// открытие файла с описанием спрайта
		//
		// пример файла:
		// texture: assets/textures/balls/Sprite-0001.png
		// textureRect: 0 0 64 64
		// position: 0 0
		// rotation: 0
		// scale: 0 0
		//

		std::ifstream fin(_filepath);
		if (!fin) WARN("sprite file: " + _filepath + " wasn't open");

		// чтение параметров
		std::string str;
		std::istringstream sstr;

		// чтение пути до текстуры
		std::getline(fin, str);
		sstr.clear();
		sstr.str(str);
		sstr >> str >> str;


		// TODO текстру нельзя удалять!!!
		(*sp).m_texture = m_texture_downloader->Download(str);
		(*sp)()->setTexture(*(*sp).m_texture);

		// чтение квадрата спрайта
		std::getline(fin, str);
		sstr.clear();
		sstr.str(str);
		sstr >> str;

		//
		// Rect<T>::Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight) :
		// left  (rectLeft),
		// top   (rectTop),
		// width (rectWidth),
		// height(rectHeight)
		//

		IntRect irect;
		sstr >> irect.left;
		sstr >> irect.top;
		sstr >> irect.width;
		sstr >> irect.height;

		// если вдруг задана ширина и высота прямоугольника больше размеров текстуры
		// изменям размер прямоугольника на размер текустуры
		if ((*sp).m_texture->getSize().x < irect.width)
		{
			INFO("texture size was changed from: "+ std::to_string(irect.width)+
				" to: "+ std::to_string((*sp).m_texture->getSize().x));
			irect.width = (*sp).m_texture->getSize().x;
		}
		if ((*sp).m_texture->getSize().y < irect.height)
		{

			INFO("texture size was changed from: " + std::to_string(irect.height) +
				" to: " + std::to_string((*sp).m_texture->getSize().y));
			irect.height = (*sp).m_texture->getSize().y;
		}

		(*sp)()->setTextureRect(irect);

		// чтение позиции спрайта
		std::getline(fin, str);
		sstr.clear();
		sstr.str(str);
		sstr >> str;

		Vector2f pos;
		sstr >> pos.x;
		sstr >> pos.y;

		(*sp)()->setPosition(pos);

		// чтение вращения
		std::getline(fin, str);
		sstr.clear();
		sstr.str(str);
		sstr >> str;

		float rot;
		sstr >> rot;

		(*sp)()->setRotation(rot);

		// чтение размера
		std::getline(fin, str);
		sstr.clear();
		sstr.str(str);
		sstr >> str;

		Vector2f sc;

		sstr >> sc.x;
		sstr >> sc.y;

		(*sp)()->setScale(sc);

		// закрытие файла
		fin.close();

		return sp;
	}
}
