#ifndef MASTER_THREAD_H
#define MASTER_THREAD_H
#include "mtpithread.h"
#include "messageprocessor.h"
class master_thread : public mtpiThread
{
public:
    master_thread(MPI_Comm group);
private:
    void run(){
        while(true){
            bool succ = MPI_P2P::listenAny();
            if(succ){
		cout<<"master node probe message from engines"<<endl;
                //MTPI_Task_Tag tag = tmp->tag();
                //int due = this->groupSize() - 1;
                std::shared_ptr<messageProcessor> th(
                            new messageProcessor(this->group(),this->groupSize()-1));
                th->start();
                th->join();
                if(th->teminate())
                    break;
            }
        }
    }
};

#endif // MASTER_THREAD_H
