#ifndef ThreadPool_h__
#define ThreadPool_h__

#include "Template.h"

using namespace Engine::Singleton;
class CThreadPool : public CSingleton<CThreadPool>
{
public:
	CThreadPool();
	~CThreadPool();

private:
	const unsigned int iMaxThreadCnt = 8;
	void Initialize();
	
	vector<thread> m_Threads;

	thread m_BgThread;
	//unique_ptr<thread> m_Thread[8];
	function<void()> m_func[8];
	std::atomic<bool> isRunning;
	mutex mtx;
	
	
	condition_variable m_condition;
	//bool isRunning;

};



#endif // TaskMaster_h__
