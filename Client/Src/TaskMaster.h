#ifndef TaskMaster_h__
#define TaskMaster_h__

#include "Template.h"

using namespace Engine::Singleton;
class CTaskMaster : public CSingleton<CTaskMaster>
{
public:
	CTaskMaster();
	~CTaskMaster();

private:
	void Initialize();
	enum ThreadLayer
	{
		TL_MAIN,TL_SUB,TL_BG,TL_MAXCOUNT
	};
	enum ThreadCount
	{
		TC_ZERO,TC_FIRST,TC_SECOND,TC_THIRD,TC_MAXCOUNT
	};

	typedef function<void()> TASK;

	queue<TASK> m_Task;

	unique_ptr<thread> m_Thread[TL_MAXCOUNT][TC_MAXCOUNT];
	function<void()> m_func[TL_MAXCOUNT][TC_MAXCOUNT];
	std::atomic<bool> isRunning;
	mutex mtx[TL_MAXCOUNT];
	
	condition_variable m_condition[TL_MAXCOUNT];
	
	//int x[TL_MAXCOUNT][TC_MAXCOUNT];//testcode

public:
	enum TaskPriority
	{
		TP_HIGH,TP_MID,TP_LOW
	};
	bool AddTask();
};



#endif // TaskMaster_h__
