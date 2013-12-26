#ifndef _THREADPOOL2_H_
#define _THREADPOOL2_H_

#include "common/sys/Thread.h"
#include "common/sys/ThreadPoolWorkItem2.h"
#include "common/sys/Mutex.h"
#include <iostream>
#include <map>

using std::cout;
using std::endl;

// 最大空闲线程数
#define THREADPOOL2_THREAD_MAX_IDLE_DEFAULT 15
// 最大线程总数
#define THREADPOOL2_THREAD_MAX_TOTAL_DEFAULT 60

class ThreadPool2
{
public:
	ThreadPool2(int maxIdle = THREADPOOL2_THREAD_MAX_IDLE_DEFAULT,
			int maxTotal = THREADPOOL2_THREAD_MAX_TOTAL_DEFAULT);
	~ThreadPool2();

	int postRequest( const ThreadPoolWorkItem2* pWorkItem );
	int postRequest( int threadID, const ThreadPoolWorkItem2* pWorkItem );
	int start();
	int stop();
protected:
	ThreadPool2( const ThreadPool2& tp );
	ThreadPool2& operator=( const ThreadPool2& tp );

	class EventHandlerThread : public Thread
	{
	public:
		EventHandlerThread();

		int DispatchItem(const ThreadPoolWorkItem2 *item);
		
	protected:
		virtual void run();
		
		int m_hHandle[2];
		
	};

	class ThreadExitWorkItem : public ThreadPoolWorkItem2
	{
	public:
        int process();
	};
	
private:
	int m_nMaxIdle;
	int m_nMaxTotal;

	std::map<int, EventHandlerThread*> m_threadMap;
	Mutex m_mutex;  // to protect m_threadMap;

	int m_nNextThread;


};
extern ThreadPool2 *g_pThreadPool2;





#endif
