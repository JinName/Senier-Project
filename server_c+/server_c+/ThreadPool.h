#pragma once
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

class ThreadPool
{
public:
	ThreadPool(size_t threadCount);
	~ThreadPool();

	void JoinThreads();

protected:
	size_t m_ThreadCount;

	std::vector<std::thread> m_ThreadPool;

	std::mutex m_Mutex;
	std::condition_variable m_CV;

	bool m_IsStopAll;

	void createThreads();

	virtual void workerThread() = 0;
};

