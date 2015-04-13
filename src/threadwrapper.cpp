#include "threadwrapper.h"
#include <unistd.h>
ThreadWrapper::ThreadWrapper():_inRunning(0),
                               _pThread(NULL),
                               _joinable(false)
{
}
/** launch the thread inside*/
void ThreadWrapper::start(){
    if(_inRunning || _joinable)
        return;
    else{
        this->_inRunning = true;
        this->_joinable = true;
        this->_pThread = new thread(runThread,(void*)this);
     }
}
/** get the thread ID*/
thread::id ThreadWrapper::self(){
    return std::this_thread::get_id();
}
/** the entry function for the thread*/
void ThreadWrapper::runThread(void* arg){
    try{
        if(!arg) throw invalid_argument("invalid thread argument");
        ((ThreadWrapper*)arg)->run();
    }catch(const exception& e){
        cerr<<e.what()<<endl;
        cerr.flush();
        exit(-1);
    }
    //_inRunning = false;
}
void ThreadWrapper::run(){
    throw 1;
}
ThreadWrapper::~ThreadWrapper(){
    if(this->_pThread){
        delete this->_pThread;
        this->_pThread = NULL;
    }
}
void ThreadWrapper::join(){
    if(_joinable){
        _pThread->join();
        _joinable = false;
    }
}
void ThreadWrapper::finish(){ this->_inRunning = false;}
