#include "Logger.h"

Logger* g_pConnLogger = nullptr;

Logger* g_pUsageLogger = nullptr;

Logger::Logger() : m_FileLogger(nullptr)
{
	InitializeCriticalSection(&m_CS);
}

Logger::~Logger()
{
	DeleteCriticalSection(&m_CS);
}

void Logger::Init(LOGGER_TYPE loggerType, const char* loggerName)
{
	switch (loggerType)
	{
	case LOGGER_TYPE::file:
		SetFileLogger(loggerName);
		break;
	}
}

bool Logger::SetFileLogger()
{
	char filename[64];
	struct tm* pTime;
	time_t long_time;

	time(&long_time);                     // �ð��� ���ͼ�
	pTime = localtime(&long_time);        // local time���� ��ȯ

	// tm_year�� -1900�� ���̰�, tm_mon�� 0���� �����̴�.
	sprintf(filename, "logs/%02d%02d%02d_log.txt",
		pTime->tm_year - 100,
		pTime->tm_mon + 1,
		pTime->tm_mday);

	// setting file logger
	m_FileLogger = spdlog::basic_logger_mt("basic_logger", filename);
	spdlog::set_default_logger(m_FileLogger);

	return true;
}

bool Logger::SetFileLogger(const char* loggerName)
{
	char filename[64];
	struct tm* pTime;
	time_t long_time;

	time(&long_time);                     // �ð��� ���ͼ�
	pTime = localtime(&long_time);        // local time���� ��ȯ

	// tm_year�� -1900�� ���̰�, tm_mon�� 0���� �����̴�.
	sprintf(filename, "logs/%02d%02d%02d_%s.txt",
		pTime->tm_year - 100,
		pTime->tm_mon + 1,
		pTime->tm_mday,
		loggerName);

	m_FileLogger = spdlog::basic_logger_mt(loggerName, filename);
	spdlog::set_default_logger(m_FileLogger);

	return true;
}

bool Logger::FileWrite(LOGGER_LEVEL level, const char* msg)
{
	if (m_FileLogger == nullptr)
	{
		printf("file logger is nullptr...\n");
		return false;
	}
	// write in file
	EnterCS();
	switch (level)
	{
	case LOGGER_LEVEL::info:
		m_FileLogger->flush_on(spdlog::level::info);
		m_FileLogger->info(msg);
		break;

	case LOGGER_LEVEL::debug:
		m_FileLogger->flush_on(spdlog::level::debug);
		m_FileLogger->debug(msg);
		break;

	case LOGGER_LEVEL::error:
		m_FileLogger->flush_on(spdlog::level::err);
		m_FileLogger->error(msg);
		break;
	}
	
	m_FileLogger->flush();
	LeaveCS();

	return true;
}

void Logger::ConsoleWrite(LOGGER_LEVEL level, const char* msg)
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
