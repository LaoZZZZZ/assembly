#ifndef THREADWRAPPER_H
#define THREADWRAPPER_H

#include <iostream>
#include <thread>
#include <cassert>
#include <stdexcept>

using std::thread;
using namespace std;
/**
  * a thread wrapper for easy use of thread
  * it could be subclassed for different task
*/
class ThreadWrapper
{
public:
    ThreadWrapper();
    template <typename T>
    ThreadWrapper(T&){}
    void start();
    void join();
    void finish();
    void yield(){std::this_thread::yield();}
    bool joinable(){return this->_joinable;}
    void enableJoin(){this->_joinable= true;}
    static void runThread(void *p);
    thread::id self();
    bool  isfinished() {return !_inRunning;}
    virtual~ThreadWrapper();
private:
    virtual void run();
protected:
    bool                _inRunning;
    thread*             _pThread;
    bool                _joinable;
};
#endif // THREADWRAPPER_H
