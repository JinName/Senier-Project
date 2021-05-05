#include "SystemUsage.h"

SystemUsage* g_pSystemUsage = nullptr;

bool SystemUsage::Init()
{
	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	m_NumProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&m_LastCPU, &ftime, sizeof(FILETIME));

	m_Handle = GetCurrentProcess();
	GetProcessTimes(m_Handle, &ftime, &ftime, &fsys, &fuser);
	memcpy(&m_LastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&m_LastUserCPU, &fuser, sizeof(FILETIME));

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

	GetProcessTimes(m_Handle, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - m_LastSysCPU.QuadPart) +
		(user.QuadPart - m_LastUserCPU.QuadPart);
	percent /= (now.QuadPart - m_LastCPU.QuadPart);
	percent /= m_NumProcessors;
	m_LastCPU = now;	
	m_LastSysCPU = sys;
	m_LastUserCPU = user;

	return percent * 100;
}