#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H
#include "mtpithread.h"
#include "lockfreequeue.h"
#include "mpi.h"
#include "mtpi_types.h"
#include "mtpi_exceptions.h"
#include "mpi_p2p.h"
/**
  * @brief a thread wrapper that could send a specific type of message
  * @brief Grap a message from the shared queue, send it and release the memory
*/
class messageSender : public mtpiThread
{
public:
    messageSender(lockFreeQueue<rawMessage*>&  que,
                  lockFreeQueue<rawMessage*>&  inque,
                  bool& finished,int maxTry,
                  MPI_Comm group,comMode m):
                    mtpiThread(group),_que(que),_inque(inque),
                    _finished(finished),_maxTry(maxTry),_mode(m){

    }
private:
    void run();

private:
    lockFreeQueue<rawMessage*>&                         _que;
    lockFreeQueue<rawMessage*>&                         _inque;
    bool&                                               _finished;
    int                                                 _maxTry;
    comMode                                             _mode;
};

#endif // MESSAGESENDER_H
