#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H
#include "mtpi_exceptions.h"
#include "mtpithread.h"
#include "lockfreequeue.h"
#include "mpi_p2p.h"
/**
  * A general message sender
*/
class messageReceiver : public mtpiThread
{
public:
    messageReceiver(lockFreeQueue<rawMessage*>& queue,bool& finished,
                    int maxtry,MPI_Comm group,comMode m):mtpiThread(group),_que(queue),_finished(finished),
                                               _maxTry(maxtry),_mode(m){
    }
private:
    void run();
private:
    lockFreeQueue<rawMessage*>&                            _que;
    bool&                                                  _finished;
    int                                                    _maxTry;
    comMode                                                _mode;
};

#endif // MESSAGERECEIVER_H
