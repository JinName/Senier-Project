#pragma once
#include <Windows.h>
#include <Psapi.h>		// 시스템 메모리 읽기위한 헤더

class SystemUsage
{
public:
	bool Init();

	double GetMemoryUsage();
	double GetCpuUsage();

private:
	// for read memory usage
	PROCESS_MEMORY_COUNTERS_EX m_Pmc;

	// for read cpu usage
	ULARGE_INTEGER m_lastCPU, m_lastSysCPU, m_lastUserCPU;
	int m_iNumProcessors;
	HANDLE m_hSelf;
};

extern SystemUsage* g_pSystemUsage;