#include "UpdateThreadPool.h"

UpdateThreadPool::UpdateThreadPool()
	:ThreadPool(THREAD_POOL_CNT), m_work_thread_cnt(0)
{
}

UpdateThreadPool::~UpdateThreadPool()
{
}

void UpdateThreadPool::workerThread()
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_CV.wait(lock, [this]() { return m_IsStopAll; });

		if (m_IsStopAll)
		{
			return;
		}

		++m_work_thread_cnt;
		lock.unlock();

		update();

		--m_work_thread_cnt;
	}
}

void UpdateThreadPool::update()
{
	auto room_list = InGameManager::GetInstance()->GetRoomListPtr();

	for (auto& room : *room_list)
	{
		if (room != nullptr)
		{
			room->Update();
		}
	}
}