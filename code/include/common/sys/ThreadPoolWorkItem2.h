#ifndef _THREADPOOLWORKITEM2_H_
#define _THREADPOOLWORKITEM2_H_

#include <stdint.h>
#include "common/comm/Error.h"

class ThreadPoolWorkItem2
{
public:
	ThreadPoolWorkItem2() {}
	virtual ~ThreadPoolWorkItem2() {}

	virtual int preProcess()
	{
		return SUCCESSFUL;
	}

	virtual int process() = 0;

	virtual int postProcess()
	{
		//g_pDispatcher2->postResult(this);
		return SUCCESSFUL;
	}

	virtual uint64_t getTaskID()
	{
		return m_taskID;
	}

	virtual void setTaskID( uint64_t id )
	{
		m_taskID = id;
	}
private:
	uint64_t m_taskID;
};


#endif
