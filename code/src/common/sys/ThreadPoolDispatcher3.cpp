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

	// set state
    m_iConnect = CONNECTED;

    

}