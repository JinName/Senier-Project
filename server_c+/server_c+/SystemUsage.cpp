#include "SystemUsage.h"

SystemUsage* g_pSystemUsage = nullptr;

bool SystemUsage::Init()
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	m_iNumProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&m_lastCPU, &ftime, sizeof(FILETIME));

	m_hSelf = GetCurrentProcess();
	GetProcessTimes(m_hSelf, &ftime, &ftime, &fsys, &fuser);
	memcpy(&m_lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&m_lastUserCPU, &fuser, sizeof(FILETIME));

	return true;
}

double SystemUsage::GetMemoryUsage()
{
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&m_Pmc, sizeof(m_Pmc));
	SIZE_T virtualMemUsedByMe = m_Pmc.PrivateUsage;

	return (double)virtualMemUsedByMe / (double)(1024 * 1024);
}

double SystemUsage::GetCpuUsage()
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(m_hSelf, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - m_lastSysCPU.QuadPart) +
		(user.QuadPart - m_lastUserCPU.QuadPart);
	percent /= (now.QuadPart - m_lastCPU.QuadPart);
	percent /= m_iNumProcessors;
	m_lastCPU = now;	
	m_lastSysCPU = sys;
	m_lastUserCPU = user;

	return percent * 100;
}