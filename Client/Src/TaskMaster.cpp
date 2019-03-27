#include "pch.h"
#include "TaskMaster.h"

CTaskMaster::CTaskMaster()
	:isRunning(true)
{
	Initialize();
}

CTaskMaster::~CTaskMaster()
{
	isRunning = false;
	for (int i = 0; i < TL_MAXCOUNT; i++)
	{
		m_condition[i].notify_all();
		for (int j = 0; j < TC_MAXCOUNT; i++)
		{
			m_Thread[i][j]->join();
		}
	}

}

void CTaskMaster::Initialize()
{
	
	for (int i = 0; i < TL_MAXCOUNT; i++)
	{
		for (int j = 0; j < TC_MAXCOUNT; j++)
		{
			m_Thread[i][j] = make_unique<thread>([&,i,j]() 
			{
				x[i][j] = 0;
				while (isRunning)
				{
					unique_lock<mutex> lock(mtx[i]);

					m_condition[i].wait(lock);

					cout << i << " " << j << endl;
					
				}
			});
		}
	}


}
