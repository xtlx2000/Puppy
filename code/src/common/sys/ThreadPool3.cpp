#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "common/sys/ThreadPool3.h"
#include "common/log/log.h"
#include "common/util/util.h"
#include "common/comm/Error.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

ThreadPool3::EventHandlerThread::
EventHandlerThread()
{
	if( NULL == g_pThreadPool3 )
	{
		DEBUG_LOG( "NULL pointer argument." );
	}

	int rt = socketpair( AF_UNIX, SOCK_STREAM, 0, m_eventfd);
	if(-1 == rt)
	{
		DEBUG_LOG( "socketpair() error." );
	}

	//trigger m_eventfd[1] EPOLLIN
	rt = ::write(m_eventfd[0], "trigger m_eventfd[1] EPOLLIN", 
			sizeof("trigger m_eventfd[1] EPOLLIN"));
	if(-1 == rt)
	{
		DEBUG_LOG( "socketpair() write error." );
	}
	//trigger m_eventfd[0] EPOLLIN
	rt = ::write(m_eventfd[1], "trigger m_eventfd[0] EPOLLIN", 
			sizeof("trigger m_eventfd[0] EPOLLIN"));
	if(-1 == rt)
	{
		DEBUG_LOG( "socketpair() write error." );
	}
}

int ThreadPool3::EventHandlerThread::
DispatchItem(const ThreadPoolWorkItem3 *item)
{
	//int rt = util::io::writen(m_hHandle[0], &item, sizeof(ThreadPoolWorkItem3 *));
	ThreadPoolWorkItem3 *pItem = const_cast<ThreadPoolWorkItem3 *>(item);
	bool ret = m_queue.push(pItem);
	if(ret != true)
	{
		return FAILED;
	}

	return SUCCESSFUL;
}

void ThreadPool3::EventHandlerThread::
run()
{
	ThreadPoolWorkItem3 *pWorkItem = NULL;

	for(; ; )
	{
		//int rt = util::io::readn( m_hHandle[1],
		//							static_cast<void*>( &pWorkItem ),
		//							sizeof( void* ) );
		bool ret = m_queue.pop(&pWorkItem);

		pthread_testcancel();
		//if(sizeof( void* ) != rt)
		//{
		//	if( EINTR != errno)
		//	{
		//		DEBUG_LOG( "Syscall Error: read. %s", strerror( errno ) );
		//	}
		//}
		if(ret != true)
		{
			DEBUG_LOG( "pop error:queue is empty.");
			::sleep(1);
			continue;
		}

		pWorkItem->preProcess();
        pWorkItem->process();
        pWorkItem->postProcess();
	}
}


ThreadPool3::ThreadPool3( int maxIdle, int maxTotal ) :
	m_nMaxIdle( maxIdle ),
    m_nMaxTotal( maxTotal ),
    m_nNextThread(0)
{
    if ( maxIdle <= 0 ||
            maxTotal <= 0 ||
            maxIdle < maxTotal )
    {
        m_nMaxIdle = THREADPOOL3_THREAD_MAX_IDLE_DEFAULT;
        m_nMaxTotal = THREADPOOL3_THREAD_MAX_TOTAL_DEFAULT;
    }
}


int ThreadPool3::postRequest(const ThreadPoolWorkItem3 * pWorkItem)
{	
	m_nNextThread = (m_nNextThread + 1) % m_nMaxIdle;

	int rt = postRequest(m_nNextThread, pWorkItem);
	
	return rt;
}

int ThreadPool3::postRequest(int threadID, const ThreadPoolWorkItem3 * pWorkItem)
{
	EventHandlerThread *eventhandlerthread = m_threadMap[threadID];
	if(eventhandlerthread == NULL)
	{
		return FAILED;
	}

	int rt = eventhandlerthread->DispatchItem(pWorkItem);

	return rt;
}

int ThreadPool3::start()
{
	for( int i = 0; i < m_nMaxIdle; ++i)
	{
		m_threadMap[i] = new EventHandlerThread();
	}
	
	for( int i = 0; i < m_nMaxIdle; ++i )
	{
		int rt = m_threadMap[i]->start();
		if(-1 == rt)
		{
			DEBUG_LOG( "Error occurred whilst starting a thread." );
            return FAILED;
		}else{
			INFO_LOG( "started a thread." );
		}
	}

	return SUCCESSFUL;
}

int ThreadPool3::stop()
{
	INFO_LOG("threadpool stop");
	if ( !m_threadMap.empty() )
	{
		for( int i = 0; i < m_nMaxIdle; ++i )
		{
			delete ( m_threadMap[i]);
		}
	}

	return SUCCESSFUL;
}

ThreadPool3::~ThreadPool3()
{
}

int ThreadPool3::ThreadExitWorkItem::
process()
{
	return SUCCESSFUL;
}
