#pragma once
#include <thread>
#include <mutex>


namespace XYZ {
	class ThreadPool
	{
	public:
		ThreadPool();



	private:
		int m_NumThreads;
		int m_ThreadsInUse;
	};
}