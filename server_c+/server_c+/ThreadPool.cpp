#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t threadCount)
	:m_ThreadCount(threadCount), m_IsStopAll(false)
{
	m_ThreadPool.reserve(threadCount);
	createThreads();
}

ThreadPool::~ThreadPool()
{
	m_IsStopAll = true;
	m_CV.notify_all();

	JoinThreads();
}


// private function
void ThreadPool::createThreads()
{
	for (size_t i = 0; i < m_ThreadCount; ++i)
	{
		m_ThreadPool.emplace_back([this]() { this->workerThread(); });
	}
}
// private function


void ThreadPool::JoinThreads()
{
	for (auto& t : m_ThreadPool)
	{
		t.join();
	}
}