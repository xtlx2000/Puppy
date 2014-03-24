Puppy
=============

A high-performance asynchronous event-driven library based on linux.


### INTRODUCTION
Ultimate performance programming framework on linux platform.

Many programming framworks (such as [libevent](http://www.libevent.org/) \ [ACE](http://www.cs.wustl.edu/~schmidt/ACE.html) \ [muduo](https://code.google.com/p/muduo/) \ [tbcommon](http://code.taobao.org/p/tb-common-utils/src/) \ our laboratory's programming framework) are not fully exploit the machine's ultimate performance, so i want to do this.


### FETURES
 * Asynchronous. refer to kylin of Baidu.
 * Event-driven. refer to reactor model.
 * High-performance. advanced architecture and advanced structure such as lock-free data structure provide better performance than libevent.
 * Thread safety. memory barrier, smart pointers, and reference counter are used to guarentee safety of concurrence.
 * Object-oriented and Object-based programming by C++.
 * GCC atomic operations.


### IS DOING
 * improve the performance of threadpool.
 * Atomic
 
### TO DO
 * multi epoll_wait
