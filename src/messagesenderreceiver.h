#ifndef MESSAGESENDERRECEIVER_H
#define MESSAGESENDERRECEIVER_H
#include "mtpithread.h"
#include "lockfreequeue.h"
#include "mpi.h"
#include "mtpi_types.h"
#include "mtpi_exceptions.h"
#include "mpi_p2p.h"
#include "messagereceiver.h"
class messageSenderReceiver:public mtpiThread
{
public:
    messageSenderReceiver(lockFreeQueue<rawMessage*>&,lockFreeQueue<rawMessage*>&,bool&,int,
                          MPI_Comm,comMode);
private:
    void run();
private:
    lockFreeQueue<rawMessage*>&              _inque;
    lockFreeQueue<rawMessage*>&              _outque;
    std::vector<rawMessage*>                 _waitMessages;
    std::vector<MPI_Request>                 _bufferedReq;
    int                                      _buffered;
    int                                      _bufferCap;
    bool&                                    _finished;
    int                                      _maxTry;
    comMode                                  _sendMode;
};

#endif // MESSAGESENDERRECEIVER_H
