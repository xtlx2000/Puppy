#ifndef _THREADPOOL3_H_
#define _THREADPOOL3_H_

#include "common/sys/Thread.h"
#include "common/sys/ThreadPoolWorkItem3.h"
#include "common/sys/Mutex.h"
#include "common/sys/DoubleQueue.h"
#include <iostream>
#include <map>

using std::cout;
using std::endl;

// �������߳���
#define THREADPOOL3_THREAD_MAX_IDLE_DEFAULT 15
// ����߳�����
#define THREADPOOL3_THREAD_MAX_TOTAL_DEFAULT 60

class ThreadPool3
{
public:
	ThreadPool3(int maxIdle = THREADPOOL3_THREAD_MAX_IDLE_DEFAULT,
			int maxTotal = THREADPOOL3_THREAD_MAX_TOTAL_DEFAULT);
	~ThreadPool3();

	int postRequest( const ThreadPoolWorkItem3* pWorkItem );
	int postRequest( int threadID, const ThreadPoolWorkItem3* pWorkItem );
	int start();
	int stop();
protected:
	ThreadPool3( const ThreadPool3& tp );
	ThreadPool3& operator=( const ThreadPool3& tp );

	class EventHandlerThread : public Thread
	{
	public:
		EventHandlerThread();

		int DispatchItem(const ThreadPoolWorkItem3 *item);
		
	protected:
		virtual void run();
		
		DoubleQueue<ThreadPoolWorkItem3 *>  m_queue;
		
	};

	class ThreadExitWorkItem : public ThreadPoolWorkItem3
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
extern ThreadPool3 *g_pThreadPool3;





#endif
