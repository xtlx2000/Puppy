#include <unistd.h>
#include <fcntl.h>

#include "common/sys/ThreadPoolDispatcher.h"
#include "common/sys/ThreadPool.h"
#include "common/log/log.h"
#include "common/comm/BaseTask.h"
#include "common/comm/TaskManager.h"
#include "common/comm/BaseHeader.h"
#include "common/util/util.h"
#include "common/comm/Epoll.h"

ThreadPoolDispatcher3::ThreadPoolDispatcher3()
{
	if( NULL == g_pEpoll ||
		NULL == g_pThreadPool )
	{
		DEBUG_LOG(
            "NULL pointer argument passed to ThreadPoolDispatcher()." );
        return;
	}


	//set epoll
	int maxIdle = g_pThreadPool3->getMaxIdle();
	
	for(int i = 0; i < maxIdle; i++)
	{
		m_readEpollEvents[i].setHandlerID( getID());// 1
		m_readEpollEvents[i].setFd(g_pThreadPool3->getEventHandlerThreadResultFd(i)); // 2
		m_readEpollEvents[i].registerREvent();// 3
		
	}

	// set state
    m_iConnect = CONNECTED;


}

ThreadPoolDispatcher3::~ThreadPoolDispatcher3()
{
    try
    {
    	int maxIdle = g_pThreadPool3->getMaxIdle();

    	for(int i = 0; i < maxIdle; i++)
    	{
    		m_readEpollEvents[i].unregisterRWEvents();
    	}
        
    }
    catch ( ... )
    {
    }
}

void
ThreadPoolDispatcher3::postRequest(
	ThreadPoolWorkItem* pWorkItem)
{
	
}

int
ThreadPoolDispatcher3::recvData()
{
	ThreadPoolWorkItem3* pWorkItem = NULL;

	int rt = 0;

	for ( ; ; )
	{
		
	}
}

