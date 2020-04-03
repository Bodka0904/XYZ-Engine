#include "stdafx.h"
#include "ThreadPool.h"



namespace XYZ {
	ThreadPool::ThreadPool()
		: m_NumThreads(std::thread::hardware_concurrency()),m_ThreadsInUse(0)
	{
	}
}