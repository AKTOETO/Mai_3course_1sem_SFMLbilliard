#ifndef LOGGER_H
#define LOGGER_H
#include "../pch.h"

// возможные цвета текста в консоли
namespace CCS // console color settings
{
	static std::string Reset = "\033[0m";

	static std::string Black = "\033[0;30m";
	static std::string Green = "\033[0;32m";
	static std::string Red = "\033[0;31m";
	static std::string White = "\033[0;37m";
	static std::string Yelow = "\033[0;33m";
}

// класс логгера 
class Logger
{
private:

	// типы логов
	enum class LogType
	{
		Info,
		Warning,
		Error
	};

	using csrc_loc = const std::source_location;

	// оболочка строк
	std::string Wrapper(
		const std::string&,
		const std::string& left_br = "[",
		const std::string& right_br = "]"
	);

	// подсвечивание и обороачивание строки
	std::string HiglightedWrapper(
		const std::string&,
		const std::string& color,
		const std::string& left_br = "[",
		const std::string& right_br = "]"
	);

	// получение строки данных
	std::string Template(csrc_loc& location);

	// получение текущего времени
	std::string GetCurrentTime();

	// получение имени файла
	std::string GetFileName(csrc_loc&);

	// печать сообщения
	void PrintMessage(LogType, const std::string&, const std::string&);

public:

	// режим логгирования
	enum class LogMode
	{
		Console,		// лог в консоль
		File,			// лог в файл
		ConsoleAndFile	// лог в консоль и в файл
	};

	Logger(LogMode);
	~Logger();
	Logger(const Logger&) = delete;

	// основные функции логгирования
	void Info(const std::string& msg = "", csrc_loc = csrc_loc::current());
	void Warning(const std::string& msg = "", csrc_loc = csrc_loc::current());
	void Error(const std::string& msg = "", csrc_loc = csrc_loc::current());

private:

	// режим работы логгера
	LogMode m_log_mode;

	// файл для вывода логов
	std::unique_ptr<std::ofstream> m_log_file;

	// путь к файлу с логами
	std::string m_log_filepath;

	// цвет каждого типа лога
	std::map<LogType, std::string> m_log_color;

	// текст каждого типа лога
	std::map<LogType, std::string> m_log_text;
};

#endif // LOGGER_H