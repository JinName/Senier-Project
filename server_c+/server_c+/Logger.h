#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

enum class LOGGER_LEVEL
{
	info, debug, error
};

enum class LOGGER_TYPE
{
	file
};

class Logger
{
public:
	Logger();
	~Logger();

	void Init(LOGGER_TYPE loggerType, const char* loggerName);

	bool SetFileLogger();
	bool SetFileLogger(const char* loggerName);
	bool FileWrite(LOGGER_LEVEL level, const char* msg);

	void ConsoleWrite(LOGGER_LEVEL level, const char* msg);

	void EnterCS() { EnterCriticalSection(&m_CS); }
	void LeaveCS() { LeaveCriticalSection(&m_CS); }
private:
	std::shared_ptr<spdlog::logger> m_FileLogger;

	CRITICAL_SECTION m_CS;
};

extern Logger* g_pConnLogger;

extern Logger* g_pUsageLogger;