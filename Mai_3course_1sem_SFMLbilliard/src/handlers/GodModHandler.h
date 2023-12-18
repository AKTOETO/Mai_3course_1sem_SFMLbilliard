#ifndef GODMODHANDLER_H
#define GODMODHANDLER_H

#include "ABCHandler.h"

// обработчик отрисовки интерфейса бога
class GodModHandler
	:public ABCHandler
{
public:
	GodModHandler(std::shared_ptr<ProgContext> context);

	// обработка логики
	virtual bool LogicHandling() override;

	// Отрисовка в окне
	virtual bool DrawHandling() override;

protected:
	// шрифт написания характеристик
	Font m_charac_desc_font;

	// список текста, который надо отрисовать для шара
	std::list<Text> m_text;

	// структура для хранения линии
	struct Line
	{
		Vertex m_line[2];
	};

	// список линий пересекшихся шаров
	std::list<Line> m_collided_lines;

};



#endif
