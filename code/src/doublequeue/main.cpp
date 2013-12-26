#include "common/comm/Epoll.h"
#include "common/comm/TCPListenAgent.h"
#include "common/comm/AgentManager.h"
#include "common/comm/TaskManager.h"
#include "common/log/log.h"
#include "common/util/util.h"
#include "common/sys/DoubleQueue.h"
#include "common/sys/Thread.h"

#include <stdlib.h>
#include <signal.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
using util::conv::conv;
using namespace std;

Epoll  *g_pEpoll  = NULL;

const int EPOLLSIZE = 1024;

DoubleQueue<int> g_dq;
static long long  readnum;
static long long  writenum;

#include <sys/time.h>

class TimeCounter
{
public:
    void begin(){gettimeofday(&_begin, NULL);}

    void end(){gettimeofday(&_end, NULL);}

    int diff()
    {
	    return ( (_end.tv_sec * 1000 + _end.tv_usec / 1000)
			- (_begin.tv_sec * 1000 + _begin.tv_usec / 1000) );
    }


private:
    struct timeval _begin;
    struct timeval _end;
};

TimeCounter tc;


void doExit(int signo)
{
    if (signo == SIGINT)       
    {
        INFO_LOG("client sigint catch!");
        
        AgentManager::getInstance()->clearAllAgent();
        TaskManager::getInstance()->clearAllTask();
        
        if ( g_pEpoll != NULL)
        {
            g_pEpoll->finish();
        }     
    }
}

class Producer : public Thread
{
public:
	void run()
	{
		bool ret;
		for(long long i = 0; i < 100000000; i++){
			//::sleep(5);
			ret = g_dq.push(i);
			if(ret){
				if(writenum % 10000000 == 0){
					cout <<">>put "<<writenum<<endl;
				}
				__sync_fetch_and_add(&writenum, 1);
			}else{
				//cout <<">>put error."<<endl;
			}
		}
		
		cout <<"swap count:"<<g_dq.m_SwapCounter<<endl;
		
	}
	
};

class Consumer : public Thread
{
public:
	void run()
	{
		bool ret;
		while(1){

			int value;
			ret = g_dq.pop(&value);
			if(ret){
				//
				__sync_fetch_and_add(&readnum, 1);
				if(readnum % 10000000 == 0){
					cout <<"<<get "<<readnum<<endl;
				}
				if(readnum == 100000000){
					tc.end();
					cout <<"Time cost:"<<tc.diff()<<endl;
				}
			}else{
				//cout <<"<<get error."<<endl;
				::sleep(1);
			}
		}
	}
};



int main(int argc, char *argv[])
{
    g_pEpoll = new Epoll();
    if ( g_pEpoll->initialize(EPOLLSIZE) == FAILED )
    {
        ERROR_LOG("\nIn main: init epoll error!\n");
        delete g_pEpoll;
        g_pEpoll = NULL;
        return FAILED;
    }

	tc.begin();
    Producer pro;
    Consumer con;
    pro.start();
    //sleep(5);
    con.start();

    

    pause();

}
