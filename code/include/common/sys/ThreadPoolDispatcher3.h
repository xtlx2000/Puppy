#ifndef _THREADPOOLDISPATCHER3_H_
#define _THREADPOOLDISPATCHER3_H_

#include <list>

#include "common/comm/Agent.h"
#include "common/comm/EpollEvent.h"
#include "common/sys/DoubleQueue.h"

class ThreadPoolWorkItem;

class ThreadPoolDispatcher3 : public Agent
{
public:
	ThreadPoolDispatcher3();
	~ThreadPoolDispatcher3();

	int init();
	void postRequest(ThreadPoolWorkItem3*);

	virtual int recvData();
	virtual int sendData();

	void postResult(const ThreadPoolWorkItem*);
	
protected:
	int setNonblock(int fd) const;
	
protected:
	ThreadPool3* m_pThreadPool;
	DoubleQueue<ThreadPoolWorkItem3*> m_queue;

	//for epoll
	EpollEvent m_readEpollEvent;
	EpollEvent m_writeEpollEvent;
	
};
extern ThreadPoolDispatcher3 *g_pDispatcher3;


#endif

