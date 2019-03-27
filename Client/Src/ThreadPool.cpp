#include "pch.h"
#include "ThreadPool.h"

CThreadPool::CThreadPool()
	:isRunning(true)
{
	Initialize();
}

CThreadPool::~CThreadPool()
{
	isRunning = false;
	
	/*for (auto j : m_Threads)
	{
		j.join();
	}*/
	
}

void CThreadPool::Initialize()
{
	for (int i = 0; i < iMaxThreadCnt; i++)
	{
		m_Threads.push_back(thread([&,i]() 
		{
			while (isRunning)
			{
				unique_lock<mutex> lock(mtx);
				m_condition.wait(lock);

				cout << i << endl;
				//m_condition.notify_one();

				
				lock.unlock();
			}
		}));
	}
	/*
	m_BgThread = thread([&]()
	{
		while (isRunning)
		{
			m_condition.wait_for()
		}
	});*/

}
