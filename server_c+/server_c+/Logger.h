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

	void Init(LOGGER_TYPE logger_type, const std::string& _loggerName);

	bool set_file_logger();
	bool set_file_logger(const std::string& loggerName);
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


//spdlog::info("spdlogger...");
//spdlog::error("Some error message with arg: {}", 1);
//
//spdlog::warn("Easy padding in numbers like {:08d}", 12);
//spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
//spdlog::info("Support for floats {:03.2f}", 1.23456);
//spdlog::info("Positional args are {1} {0}..", "too", "supported");
//spdlog::info("{:<30}", "left aligned");
//
//auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
//spdlog::set_default_logger(file_logger);
//
//file_logger->flush_on(spdlog::level::err);
//file_logger->error("debug message1");
//file_logger->error("debug message2");
//file_logger->error("debug message3");
//file_logger->error("debug message4");
//file_logger->flush();