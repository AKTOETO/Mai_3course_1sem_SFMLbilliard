#include "BallObjectDownloadManager.h"

namespace Engine
{
	std::shared_ptr<Ball> Engine::BallObjectDownloadManager::Download(const std::string& _filepath)
	{
		// создание шара
		std::shared_ptr<Ball> ball(new Ball(GetFreeId()));

		// считывание характеристик шара
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

				float num;
				Vector2f vec;

				// чтение массы
				if (str == "mass")
				{
					instr >> num;
					ball->SetMass(num);
				}
				// чтение позиции
				else if (str == "position")
				{
					instr >> vec.x >> vec.y;
					ball->SetPos(vec);
				}
				// чтение радиуса
				else if (str == "radius")
				{
					instr >> num;
					ball->SetRadius(num);
				}
				// чтение спрайта
				else if (str == "sprite")
				{
					std::string sprite_fp;
					instr >> sprite_fp;
					ball->SetSprite(m_sprite_download->Download(sprite_fp));
				}
			}
		}

		return ball;
	}
}
