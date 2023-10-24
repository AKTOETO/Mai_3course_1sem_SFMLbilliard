#ifndef CONTEXT_H
#define CONTEXT_H

#include "pch.h"
#include "logger/Logger.h"

namespace Engine
{
	// контекст программы
	struct Context
	{
		// логгер
		std::unique_ptr<Logger> m_log;

#define INFO(msg) m_context->m_log->Info(msg)
#define WARN(msg) m_context->m_log->Warning(msg)
#define ERR(msg) m_context->m_log->Error(msg)

		// конструктор
		Context() :
			m_log(std::make_unique<Logger>(Logger::LogMode::ConsoleAndFile))
		{}
	};
}

#endif // !CONTEXT_H
