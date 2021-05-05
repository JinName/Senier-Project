#pragma once

#include "ThreadPool.h"
#include "InGameManager.h"

#define THREAD_POOL_CNT 12
#define THREAD_CONTROL_ROOM_CNT 200 // thread 1 개당 컨트롤하는 room 갯수

class UpdateThreadPool : public ThreadPool
{
public:
	UpdateThreadPool();
	~UpdateThreadPool();

protected:
	virtual void workerThread();

	void update();

private:
	size_t m_room_cnt;
	size_t m_work_thread_cnt;
};

