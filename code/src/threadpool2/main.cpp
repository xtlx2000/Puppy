#include "common/comm/Epoll.h"
#include "common/comm/TCPListenAgent.h"
#include "common/comm/AgentManager.h"
#include "common/comm/TaskManager.h"
#include "common/log/log.h"
#include "common/util/util.h"
#include "common/sys/Thread.h"

#include "common/sys/ThreadPool2.h"


#include <stdlib.h>
#include <signal.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
using util::conv::conv;
using namespace std;


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

Epoll  *g_pEpoll  = NULL;
ThreadPool2 *g_pThreadPool2 = NULL;
long long  itemnum;



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

class TestItem : public ThreadPoolWorkItem2
{
public:
	int process()
	{
		__sync_fetch_and_add(&itemnum, 1);
		if(itemnum % 100000 == 0){
			cout <<"num="<<itemnum<<endl;
		}
		if(itemnum == 9999999){
			tc.end();
			cout <<"time:"<<tc.diff()<<endl;
			exit(0);
		}
		return 0;
	}
	int postProcess()
	{
		delete this;
		return 0;
	}
};

int main(int argc, char *argv[])
{
    g_pEpoll = new Epoll();
    if ( g_pEpoll->initialize(10000) == FAILED )
    {
        ERROR_LOG("\nIn main: init epoll error!\n");
        delete g_pEpoll;
        g_pEpoll = NULL;
        return FAILED;
    }

    g_pThreadPool2 = new ThreadPool2();
    g_pThreadPool2->start();

	tc.begin();
	for(int i = 0; i < 10000000; i++){
		ThreadPoolWorkItem2 *item1 = new TestItem();
    	g_pThreadPool2->postRequest( item1);
    }
    

    pause();

}
