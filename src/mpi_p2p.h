/**
  * @file MPI_P2P file
  * @brief a wrapper of MPI point to point
  * @author Lu Zhao luzhao1986@gmail.com
*/
#ifndef MPI_P2P_H
#define MPI_P2P_H
#include <cassert>
#include "mpi.h"
#include "rawmessage.h"

class MPI_P2P
{
private:
    MPI_P2P();
public:
    /**
      * listen to any incoming message
    */
    inline static bool listenAny(){
        MPI_Status status;
        int flag;
        MPI_Iprobe(MPI_ANY_SOURCE,UNKNOWN_TAG,MPI_COMM_WORLD,&flag,&status);
        return flag;
    }
    /**
      * Listen to a specific type message
      * If the message does not have the specific tag, then
      * just discard it
      */
    inline static bool listenSpecificTag(MTPI_Task_Tag tag,MPI_Comm group = MPI_COMM_WORLD){
        MPI_Status status;
        int flag = 0;
        MPI_Iprobe(MPI_ANY_SOURCE,tag,group,&flag,&status);
        return flag;
    }

    /**
      * Listen to a specific type message
      * If the message does not have the specific tag, then
      * just discard it
      */
    inline static bool listenSpecificRank(int rank,MPI_Comm group = MPI_COMM_WORLD){
        MPI_Status status;
        int flag = 0;

        MPI_Iprobe(rank,UNKNOWN_TAG,group,&flag,&status);
        return flag;
    }
    /**
     * @brief get my process rank
    */
    inline static int getRank(MPI_Comm group = MPI_COMM_WORLD){
        int rank = -1;
        MPI_Comm_rank(group,&rank);
        return rank;
    }
    /**
      * Blocking send function
    */
    inline static bool Send(rawMessage* msg,MPI_Comm group = MPI_COMM_WORLD){
        if(msg){
            const char* data = msg->message().c_str();
            int sz = msg->size();
            int dest = msg->destination();
            if(MPI_SUCCESS == MPI_Send((void *)data,sz,MPI_CHAR,dest,(int)(msg->tag()),group))
                return true;
            }
        return true;
    }
    /**
      * Non blocking send function
      *
    */
    inline static MPI_Request ISend(rawMessage* msg,MPI_Comm group = MPI_COMM_WORLD){
        assert(msg);

        int sz = msg->size();
        int dest = msg->destination();
        MPI_Request request;
        //MPI_Status  status;
        MPI_Isend((void *)(msg->message().c_str()),   // data to send
                  sz,                                  // data size
                  MPI_CHAR,                           // data type
                  dest,                              // destination
                  (int)(msg->tag()),              // message tag
                  group,                           // communication group
                  &request
                  );
        return request;

    }
    /**
      * Blocking receive message function
      * Basically it graps message from the buffer whateve it comes out first
    */
    inline static rawMessage* RecvAny(bool& succ,MPI_Comm group = MPI_COMM_WORLD){
        MPI_Status status;
        int size = 0;
	int flag = 0;
        MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,group,&flag,&status);
	if(!flag){
		succ = false;
		return NULL;
	}
        MPI_Get_count(&status,MPI_CHAR,&size);
        char* buf = new char[size+1];
        memset(buf,0,size+1);
        int s = MPI_Recv(buf,size,MPI_CHAR,status.MPI_SOURCE,status.MPI_TAG,group,&status);
        succ = (s == MPI_SUCCESS);
        rawMessage* mesg = NULL;
        if(succ){
        	string cont;
        	cont.assign(buf,size);
        	mesg = new rawMessage(status.MPI_SOURCE,MPI_P2P::getRank(group),cont,(MTPI_Task_Tag)(status.MPI_TAG));
        }
        delete []buf;
        buf = NULL;
        return mesg;
    }

    /**
      * Blocking receive message function
      * Basically it graps message from the buffer whateve it comes out first
    */
    inline static rawMessage* Recv(int src,bool& succ,MPI_Comm group = MPI_COMM_WORLD){
        MPI_Status status;
        int size = 0;
        MPI_Probe(src,MPI_ANY_TAG,group,&status);
        assert(status.MPI_SOURCE == src);
        MPI_Get_count(&status,MPI_CHAR,&size);
        char* buf = new char[size+1];
        memset(buf,0,size+1);
        int s = MPI_Recv(buf,size,MPI_CHAR,status.MPI_SOURCE,status.MPI_TAG,group,&status);
        succ = (s == MPI_SUCCESS);
        rawMessage* mesg = NULL;
        if(succ){
            string cont;
            cont.assign(buf,size);
            mesg = new rawMessage(status.MPI_SOURCE,MPI_P2P::getRank(group),cont,(MTPI_Task_Tag)(status.MPI_TAG));
        }
        delete []buf;
        buf = NULL;
        return mesg;
    }
    /**
      * Non blocking receive message function
      * Basically it graps the message from the buffer whatever it comes out first
    */
    inline static rawMessage* IRecvAny(bool& succ,MPI_Comm group = MPI_COMM_WORLD){
        MPI_Status status;
        MPI_Request request;
        int flag = 0;
        int size = 0;
        rawMessage* mesg = NULL;

        MPI_Iprobe(MPI_ANY_SOURCE,MPI_ANY_TAG,group,&flag,&status);
        if(flag){
            MPI_Get_count(&status,MPI_CHAR,&size);
            char* buf = new char[size+1];
            memset(buf,0,size+1);
            int s = MPI_Irecv((void*)buf,size,MPI_CHAR,status.MPI_SOURCE,status.MPI_TAG,group,&request);
	    if(s != MPI_SUCCESS){
		succ = false;
		return mesg;
	    }
		
            s = MPI_Wait(&request,&status);
            succ = (MPI_SUCCESS ==s);
            string cont;
            cont.assign(buf,size);
	    if(!succ){
		cout<<cont.size()<<'\t'<<status.MPI_ERROR<<endl;
	    }
            mesg = new rawMessage(status.MPI_SOURCE,MPI_P2P::getRank(group),cont,(MTPI_Task_Tag)(status.MPI_TAG));

            delete []buf;
            buf = NULL;
        }else{
            succ = false;
        }
        return mesg;
    }

    /**
      * Non blocking receive message function
      * Basically it graps the message from the buffer whatever it comes out first
    */
    inline static rawMessage* IRecv(int src,bool& succ,MTPI_Task_Tag tag = UNKNOWN_TAG,MPI_Comm group = MPI_COMM_WORLD){
        MPI_Status status;
        MPI_Request request;
        int flag = 0;
        int size = 0;
        rawMessage* mesg = NULL;

        MPI_Iprobe(src,tag,group,&flag,&status);
        if(flag){
            MPI_Get_count(&status,MPI_CHAR,&size);
            assert(status.MPI_SOURCE == src);
            char* buf = new char[size+1];
            memset(buf,0,size+1);
            MPI_Irecv((void*)buf,size,MPI_CHAR,status.MPI_SOURCE,status.MPI_TAG,group,&request);
            succ = (MPI_SUCCESS == MPI_Wait(&request,&status));
            if(succ){
                string cont;
                cont.assign(buf,size);
                mesg = new rawMessage(status.MPI_SOURCE,MPI_P2P::getRank(group),cont,(MTPI_Task_Tag)(status.MPI_TAG));
        }
        delete []buf;
        buf = NULL;
        }
        return mesg;
    }
};

#endif // MPI_P2P_H
