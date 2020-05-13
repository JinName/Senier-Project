#include "Logger.h"

Logger* g_pLogger = nullptr;

Logger* g_pUsageLogger = nullptr;

Logger::Logger() : file_logger(nullptr)
{
	InitializeCriticalSection(&mCS);
}

Logger::~Logger()
{
	DeleteCriticalSection(&mCS);
}

void Logger::Init(LOGGER_TYPE logger_type, const std::string& _loggerName)
{
	switch (logger_type)
	{
	case LOGGER_TYPE::file:
		set_file_logger(_loggerName);
		break;
	}
}

bool Logger::set_file_logger()
{
	char filename[64];
	struct tm* pTime;
	time_t long_time;

	time(&long_time);                     // 시간을 얻어와서
	pTime = localtime(&long_time);        // local time으로 전환

	// tm_year은 -1900된 값이고, tm_mon은 0부터 시작이다.
	sprintf(filename, "logs/%02d%02d%02d_log.txt",
		pTime->tm_year - 100,
		pTime->tm_mon + 1,
		pTime->tm_mday);

	// setting file logger
	file_logger = spdlog::basic_logger_mt("basic_logger", filename);
	spdlog::set_default_logger(file_logger);

	return true;
}

bool Logger::set_file_logger(const std::string& _loggerName)
{
	char filename[64];
	struct tm* pTime;
	time_t long_time;

	time(&long_time);                     // 시간을 얻어와서
	pTime = localtime(&long_time);        // local time으로 전환

	// tm_year은 -1900된 값이고, tm_mon은 0부터 시작이다.
	sprintf(filename, "logs/%02d%02d%02d_%s.txt",
		pTime->tm_year - 100,
		pTime->tm_mon + 1,
		pTime->tm_mday,
		_loggerName);

	file_logger = spdlog::basic_logger_mt(_loggerName, filename);
	spdlog::set_default_logger(file_logger);

	return true;
}

bool Logger::file_write(LOGGER_LEVEL level, const char* msg)
{
	if (file_logger == nullptr)
	{
		printf("file logger is nullptr...\n");
		return false;
	}
	// write in file
	EnterCS();
	switch (level)
	{
	case LOGGER_LEVEL::info:
		file_logger->flush_on(spdlog::level::info);
		file_logger->info(msg);
		break;

	case LOGGER_LEVEL::debug:
		file_logger->flush_on(spdlog::level::debug);
		file_logger->debug(msg);
		break;

	case LOGGER_LEVEL::error:
		file_logger->flush_on(spdlog::level::err);
		file_logger->error(msg);
		break;
	}
	
	file_logger->flush();
	LeaveCS();

	return true;
}

void Logger::console_write(LOGGER_LEVEL level, const char* msg)
{
	switch (level)
	{
	case LOGGER_LEVEL::info:
		spdlog::info(msg);
		break;

	case LOGGER_LEVEL::debug:
		spdlog::debug(msg);
		break;

	case LOGGER_LEVEL::error:
		spdlog::error(msg);
		break;
	}
}
