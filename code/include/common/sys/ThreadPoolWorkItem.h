#ifndef _THREADPOOLWORKITEM_H_
#define _THREADPOOLWORKITEM_H_

#include <stdint.h>

#include "common/sys/ThreadPoolDispatcher.h"

/**
 * @brief �̳߳�������ӿ�
 */
class ThreadPoolWorkItem
{
public:
    ThreadPoolWorkItem() {}
    virtual ~ThreadPoolWorkItem() {}

    /**
     * @brief ��ִ�о���ҵ��ǰ�Ĺ�������
     *
     * @return
     */
    virtual int preProcess()
    {
        return SUCCESSFUL;
    }

    /**
     * @brief ʵ�ִ˷�����ִ�о���ҵ��
     *
     * @return
     */
    virtual int process() = 0;

    /**
     * @brief ִ�о���ҵ���Ĺ�������
     *
     * @return
     */
    virtual int postProcess()
    {
        g_pDispatcher->postResult( this );
        return SUCCESSFUL;
    }

    virtual uint64_t getTaskID()
    {
        return m_taskID;
    }

    virtual void setTaskID( uint64_t id )
    {
        m_taskID = id;
        return;
    }

private:
    uint64_t m_taskID;
};

#endif // _THREADPOOLWORKITEM_H_
