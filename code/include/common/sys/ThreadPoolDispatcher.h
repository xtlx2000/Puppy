/*
 * =====================================================================================
 *
 *       Filename:  ThreadPoolDispatcher.h
 *
 *    Description:  �̳߳�����ת��
 *
 *        Version:  1.0
 *        Created:  04/27/2011 11:18:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#ifndef _THREADPOOL_DISPATCHER_H_
#define _THREADPOOL_DISPATCHER_H_

#include <list>

#include "common/comm/Agent.h"
#include "common/comm/EpollEvent.h"

class ThreadPoolWorkItem;

/**
 * @brief Dispatcher������ά��һ���ܵ���
 * �����̳߳��е����񽫽��д���ϲ㣬
 * �˹ܵ��Ķ���ע�ᵽEpoll��
 * ���⣬��ȡ�̳߳عܵ���д�ˣ�����ע�ᵽEpoll��
 * �������Է������ؽ������·����̳߳ء�
 */
class ThreadPoolDispatcher : public Agent
{
public:
    ThreadPoolDispatcher();
    ~ThreadPoolDispatcher();

	int init();

    /**
     * @brief ���߳��ڵ��ô˺�����������ŵ�����������б���,
     *        ��ThreadPool�еĹܵ�������д�¼�ʱ�������threapool
     *        ��postRequest����ͨ����ܵ��������·����̳߳��С�
     *
     * @param pWorkItem
     */
    void postRequest( ThreadPoolWorkItem* );

    /**
     * @brief ���¼��������ӹܵ���ȡ������
     *
     * @return
     */
    virtual int recvData();

    /**
     * @brief д�¼��������������·����̳߳�
     *
     * @return
     */
    virtual int sendData();

    /**
     * @brief �̳߳��ڵ��ô˺���,�������д�����߳�
     *
     * @return
     */
    void postResult( const ThreadPoolWorkItem* );

protected:
    /**
     * @brief ����������Ϊ������
     *
     * @param fd ������
     *
     * @return
     */
    int setNonblock( int fd ) const;

protected:
    int m_hThreadPoolHandle;
    int m_hReadHandle;
    int m_hWriteHandle;
    std::list<ThreadPoolWorkItem*> m_workItemList;

    // for epoll
    EpollEvent m_readEpollEvent;
    EpollEvent m_writeEpollEvent;
};
extern ThreadPoolDispatcher* g_pDispatcher;

#endif  // _THREADPOOL_DISPATCHER_H_

