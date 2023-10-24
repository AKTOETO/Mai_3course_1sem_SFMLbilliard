#include "pch.h"
#include "SpriteDownloadManger.h"

namespace Engine
{
	std::shared_ptr<CustomSprite> Engine::SpriteDownloadManager::Download(const std::string& _filepath)
	{
		std::shared_ptr<CustomSprite> sp;

		sp = Download(_filepath, nullptr);	

		return sp;
	}

	std::shared_ptr<CustomSprite> SpriteDownloadManager::Download(const std::string& _filepath, std::shared_ptr<Texture> _texture)
	{
		// создание спрайта и установка текстуры
		std::shared_ptr<CustomSprite> sp(new CustomSprite);
		//sp->m_texture = _texture;
		sp->SetTexture(_texture);

		//
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
		// если файл не был открыт
		if (!fin)
			WARN("sprite file: " + _filepath + " wasn't open");
		// чтение параметров
		else
		{
			// проходимся по каждой строке
			for (std::string str; getline(fin, str);)
			{
				std::istringstream instr(str);

				// определяем тип вводимых данных
				instr >> str;

				// в зависимости от типа, считываем данных

				// чтение текстуры
				if (str == "texture" && !(*sp).m_texture)
				{
					instr >> str;

					sp->SetTexture(m_texture_downloader->Download(str));
					//(*sp).m_texture = m_texture_downloader->Download(str);
					//(*sp)()->setTexture(*(*sp).m_texture);
				}
				// чтение квадрата спрайта
				else if (str == "textureRect")
				{
					//
					// Rect<T>::Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight) :
					// left  (rectLeft),
					// top   (rectTop),
					// width (rectWidth),
					// height(rectHeight)
					//

					IntRect irect;
					instr >> irect.left;
					instr >> irect.top;
					instr >> irect.width;
					instr >> irect.height;

					// если вдруг задана ширина и высота прямоугольника больше размеров текстуры
					// изменям размер прямоугольника на размер текустуры
					if ((*sp).m_texture->getSize().x < irect.width)
					{
						INFO("texture size was changed from: " + std::to_string(irect.width) +
							" to: " + std::to_string((*sp).m_texture->getSize().x));
						irect.width = (*sp).m_texture->getSize().x;
					}
					if ((*sp).m_texture->getSize().y < irect.height)
					{

						INFO("texture size was changed from: " + std::to_string(irect.height) +
							" to: " + std::to_string((*sp).m_texture->getSize().y));
						irect.height = (*sp).m_texture->getSize().y;
					}

					(*sp)()->setTextureRect(irect);
				}
				// чтение позиции спрайта
				else if (str == "position")
				{
					Vector2f pos;
					instr >> pos.x;
					instr >> pos.y;

					(*sp)()->setPosition(pos);
				}
				// чтение вращения
				else if (str == "rotation")
				{
					float rot;
					instr >> rot;

					(*sp)()->setRotation(rot);
				}
				// чтение размера
				else if (str == "scale")
				{
					Vector2f sc;

					instr >> sc.x;
					instr >> sc.y;

					(*sp)()->setScale(sc);
				}
			}
		}

		// закрытие файла
		fin.close();

		return sp;
	}
}
