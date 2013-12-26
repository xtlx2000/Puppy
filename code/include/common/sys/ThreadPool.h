/*
 * =====================================================================================
 *
 *       Filename:  ThreadPool.h
 *
 *    Description:  �̳߳�ʵ��
 *
 *        Version:  1.0
 *        Created:  04/26/2011 11:11:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shi Wei (sw), shiwei2012@gmail.com
 *        Company:
 *
 * =====================================================================================
 */

#ifndef COMMON_SYS_THREADPOOL_H_
#define COMMON_SYS_THREADPOOL_H_

#include "common/sys/Thread.h"
#include "common/sys/ThreadPoolWorkItem.h"
#include "common/sys/Mutex.h"
#include <iostream>
using std::cout;
using std::endl;

// �������߳���
#define THREAD_MAX_IDLE_DEFAULT 15
// ����߳�����
#define THREAD_MAX_TOTAL_DEFAULT 60

/**
 * @brief �̳߳�ʵ��
 */
class ThreadPool
{
public:
    /**
     * @brief ���캯��
     *
     * @param maxIdle �������߳���
     * @param maxTotal ����߳�����
     */
    ThreadPool( int maxIdle = THREAD_MAX_IDLE_DEFAULT,
                int maxTotal = THREAD_MAX_TOTAL_DEFAULT );

    ~ThreadPool();

    /**
     * @brief �·������̳߳أ���д��m_hWriteHandle
     *
     * @param pWorkItem �̳߳�����ָ��
     *
     * @return
     *      - 0 �ɹ�
     *      - ... ʧ�ܵ�errno
     */
    int postRequest( const ThreadPoolWorkItem* pWorkItem );

    /**
     * @brief �����̳߳ؾ������m_hWriteHandle
     *
     * @return �̳߳ؾ��
     */
    int getWHandle() const;
	int getRHandle() const;

    /**
     * @brief �����̳߳�
     *
     * @return
     */
    int start();

    /**
     * @brief ֹͣ�̳߳�
     *
     * @return
     */
    int stop();

protected:
    //can't copy
    ThreadPool( const ThreadPool& tp );
    ThreadPool& operator=( const ThreadPool& tp );

    /**
     * @brief �̳߳���ִ���¼�������߳�
     */
    class EventHandlerThread : public Thread
    {
    public:
        EventHandlerThread();

    protected:
        virtual void run();
    };

    /**
     * @brief �̳߳��յ����������˳�
     */
    class ThreadExitWorkItem : public ThreadPoolWorkItem
    {
    public:
        int process();
    };

private:
    int m_hReadHandle;  // �ܵ�����
    int m_hWriteHandle;  // �ܵ�д��
    int m_nMaxIdle;
    int m_nMaxTotal;
    std::list<Thread*> m_threadList;
    Mutex m_mutex;  // to protect m_threadList;
};
extern ThreadPool *g_pThreadPool;

#endif  // COMMON_SYS_THREADPOOL_H_

