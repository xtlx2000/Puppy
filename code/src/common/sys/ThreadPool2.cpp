#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "common/sys/ThreadPool2.h"
#include "common/log/log.h"
#include "common/util/util.h"
#include "common/comm/Error.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

ThreadPool2::EventHandlerThread::
EventHandlerThread()
{
	if( NULL == g_pThreadPool2 )
	{
		DEBUG_LOG( "NULL pointer argument." );
	}

	int rt = socketpair( AF_UNIX, SOCK_STREAM, 0, m_hHandle);
	//int rt = pipe(m_hHandle);
	if(-1 == rt)
	{
		DEBUG_LOG( "socketpair() error." );
	}
}

int ThreadPool2::EventHandlerThread::
DispatchItem(const ThreadPoolWorkItem2 *item)
{
	int rt = util::io::writen(m_hHandle[0], &item, sizeof(ThreadPoolWorkItem2 *));
	if(rt != sizeof(ThreadPoolWorkItem2 *))
	{
		return FAILED;
	}

	return SUCCESSFUL;
}

void ThreadPool2::EventHandlerThread::
run()
{
	ThreadPoolWorkItem2 *pWorkItem = NULL;

	for(; ; )
	{
		int rt = util::io::readn( m_hHandle[1],
									static_cast<void*>( &pWorkItem ),
									sizeof( void* ) );

		pthread_testcancel();
		if(sizeof( void* ) != rt)
		{
			if( EINTR != errno)
			{
				DEBUG_LOG( "Syscall Error: read. %s", strerror( errno ) );
			}
		}

		pWorkItem->preProcess();
        pWorkItem->process();
        pWorkItem->postProcess();
	}
}


ThreadPool2::ThreadPool2( int maxIdle, int maxTotal ) :
	m_nMaxIdle( maxIdle ),
    m_nMaxTotal( maxTotal ),
    m_nNextThread(0)
{
    if ( maxIdle <= 0 ||
            maxTotal <= 0 ||
            maxIdle < maxTotal )
    {
        m_nMaxIdle = THREADPOOL2_THREAD_MAX_IDLE_DEFAULT;
        m_nMaxTotal = THREADPOOL2_THREAD_MAX_TOTAL_DEFAULT;
    }
}


int ThreadPool2::postRequest(const ThreadPoolWorkItem2 * pWorkItem)
{	
	m_nNextThread = (m_nNextThread + 1) % m_nMaxIdle;

	int rt = postRequest(m_nNextThread, pWorkItem);
	
	return rt;
}

int ThreadPool2::postRequest(int threadID, const ThreadPoolWorkItem2 * pWorkItem)
{
	EventHandlerThread *eventhandlerthread = m_threadMap[threadID];
	if(eventhandlerthread == NULL)
	{
		return FAILED;
	}

	int rt = eventhandlerthread->DispatchItem(pWorkItem);

	return rt;
}

int ThreadPool2::start()
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

int ThreadPool2::stop()
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

ThreadPool2::~ThreadPool2()
{
}

int ThreadPool2::ThreadExitWorkItem::
process()
{
	return SUCCESSFUL;
}