#include "Logger.h"

std::string Logger::Wrapper(
	const std::string& data,
	const std::string& left_br,
	const std::string& right_br
)
{
	return left_br + data + right_br;
}

std::string Logger::HiglightedWrapper(
	const std::string& data,
	const std::string& color,
	const std::string& left_br,
	const std::string& right_br
)
{
	return color + left_br + data + right_br + CCS::Reset;
}

// формирование строки данных
std::string Logger::Template(csrc_loc& loc)
{
	// [time][file_name:line:column][func_name][data]
	std::string str =
		Wrapper(GetCurrentTime()) +
		Wrapper(
			GetFileName(loc) + ":" +
			std::to_string(loc.line()) + ":" +
			std::to_string(loc.column())
		) +
		Wrapper(loc.function_name());

	return str;
}

std::string Logger::GetCurrentTime()
{
	time_t now = time(0);
	tm* tstruct = new tm;
	char buf[9];
	localtime_s(tstruct, &now);
	strftime(buf, sizeof(buf), "%H:%M:%S", tstruct);

	return std::string(buf);
}

std::string Logger::GetFileName(csrc_loc& loc)
{
	std::string file_name = loc.file_name();
	size_t pos = file_name.find_last_of("\\") + 1;
	return file_name.substr(pos);
}

void Logger::PrintMessage(
	LogType type,
	const std::string& data,
	const std::string& msg
)
{
	switch (m_log_mode)
	{
	case Logger::LogMode::Console:

#ifdef _DEBUG
		std::cout <<
			HiglightedWrapper(m_log_text[type], m_log_color[type]) <<
			data <<
			HiglightedWrapper(msg, CCS::Green) <<
			std::endl;
#endif // _DEBUG
		break;

	case Logger::LogMode::ConsoleAndFile:
#ifdef _DEBUG
		std::cout <<
			HiglightedWrapper(m_log_text[type], m_log_color[type]) <<
			data <<
			HiglightedWrapper(msg, CCS::Green) <<
			std::endl;
#endif // _DEBUG

	case Logger::LogMode::File:
		*m_log_file <<
			Wrapper(m_log_text[type]) <<
			data <<
			Wrapper(msg) <<
			std::endl;
		break;
	}
}

Logger::Logger(LogMode _log_mode)
	: m_log_mode(_log_mode),
	m_log_filepath("log/log.txt")
{
	// настройка цвета логов
	m_log_color =
	{
		{LogType::Info, CCS::Green},
		{LogType::Warning, CCS::Yelow},
		{LogType::Error, CCS::Red},
	};

	// настройка текста лога
	m_log_text =
	{
		{LogType::Info, "INFO"},
		{LogType::Warning, "WARNING"},
		{LogType::Error, "ERROR"},
	};

	// если необходимо выводить в файл, создаем файловый поток
	switch (m_log_mode)
	{
	case Logger::LogMode::File:
	case Logger::LogMode::ConsoleAndFile:

		// открытие файла для записи логов
		m_log_file = std::make_unique<std::ofstream>(m_log_filepath);

		// если файл не был открыт
		if (!*m_log_file) Warning("log file wasn't open");

		break;
	}
}

Logger::~Logger()
{
	if (m_log_file)m_log_file->close();
}

void Logger::Info(const std::string& msg, csrc_loc location)
{
	PrintMessage(LogType::Info, Template(location), msg);
}

void Logger::Warning(const std::string& msg, csrc_loc location)
{
	PrintMessage(LogType::Warning, Template(location), msg);
}

void Logger::Error(const std::string& msg, csrc_loc location)
{
	PrintMessage(LogType::Error, Template(location), msg);
}
