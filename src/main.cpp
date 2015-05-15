#include "mpi.h"
#include <stdio.h>
#include <iostream>
#include "lockfreequeue.h"
#include "threadwrapper.h"
#include "mtpi_funcs.h"
#include "mpi_p2p.h"
#include "generatetable.h"
#include "master_thread.h"
#include "engine_thread.h"
class testthread : public ThreadWrapper{
public:
    testthread():ThreadWrapper(){

    }
private:
    void run(){
        cout<<std::this_thread::get_id()<<endl;
        cout.flush();
    }

};
using namespace std;

int main(int argc,char** argv)
{
    // test
    int rank,size;
    int provide = 0;
    int numThread = 0;
    MPI_Init_thread(&argc,&argv,MPI_THREAD_MULTIPLE,&provide);
    if(provide < MPI_THREAD_MULTIPLE){
        cerr<<"The MPI platform does not support multiple thread. it only supports single thread mode"<<endl;
        numThread = 1;
    }else{
        cerr<<"The MPI platform support multiple threads!Horray"<<endl;
        numThread = 2;
    }
    numThread = 1;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    AssemblyParameter* ap = new AssemblyParameter();
    globalParameter* gp = new globalParameter();
    gp->setnp(size);
    if(argc >= 2)
        gp->setPrefixLen(atoi(argv[1]));
    if(argc >= 3)
        gp->setKmerLen(atoi(argv[2]));
    if(argc >= 4){
        gp->setCommunicationMode(static_cast<comMode>(atoi(argv[3])));
    }
    gp->setCommunicationThread(numThread);
    Parameter* p = new Parameter(ap,gp);
   std::shared_ptr<Parameter> param(p);
    mtpiThread* th = NULL;
    if(rank == 0){
        th = new master_thread(param->globalParameters()->comGroup());
        th->start();
        th->join();
	}
    else{
        th = new engine_thread(param);
        th->start();
        th->join();
	}
    if(th)
        delete th;
    th = NULL;
    MPI_Finalize();
    return 0;
}

