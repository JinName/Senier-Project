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

	void Init(LOGGER_TYPE logger_type, const char* _loggerName);

	bool set_file_logger();
	bool set_file_logger(const char* loggerName);
	bool file_write(LOGGER_LEVEL level, const char* msg);

	void console_write(LOGGER_LEVEL level, const char* msg);

	void EnterCS() { EnterCriticalSection(&mCS); }
	void LeaveCS() { LeaveCriticalSection(&mCS); }
private:
	std::shared_ptr<spdlog::logger> file_logger;

	CRITICAL_SECTION mCS;
};

extern Logger* g_pConnLogger;

extern Logger* g_pUsageLogger;