#ifndef MTPITHREAD_H
#define MTPITHREAD_H
#include "mpi.h"
#include "threadwrapper.h"

/**
  * a base class of working thread at each node
  * Basically, this thread belongs to a mpi process and communication group
  *
*/
class mtpiThread : public ThreadWrapper
{
public:
    mtpiThread(MPI_Comm group);
    int getRank()const{
        int me = 0;
        MPI_Comm_rank(_group,&me);
        return me;
    }
    int groupSize()const{
        int sz = 0;
        MPI_Comm_size(_group,&sz);
        return sz;
    }
    MPI_Comm group()const{return this->_group;}
    virtual ~mtpiThread(){}
private:
    MPI_Comm                    _group;
};

#endif // MTPITHREAD_H
