#include "messagereceiver.h"

void messageReceiver::run(){
    if(this->_mode == NONBLOCKING){
        while(!this->_finished){
            bool ack;
            // non blocking receive message from the channel
            rawMessage* tmp = MPI_P2P::IRecvAny(ack,this->group());
            if(!ack || !tmp){ // if there is no message or the message is empty
                continue;
            }
            cout<<this->getRank()<<" receive message from " << tmp->source()<<endl;
            // put the message to the shared queue
            int i = 0;
            bool success = this->_que.tryAdd(tmp);
            while(!success && i < this->_maxTry){
                success = this->_que.tryAdd(tmp);
                i++;
            }
            if(!success){
                stringstream ss;
                ss << tmp->source()<<"->"<<tmp->destination()<<"message ..";
                throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
            }

        }
    }
    else{
        while(!this->_finished){
            bool ack;
            // non blocking receive message from the channel
            rawMessage* tmp = MPI_P2P::RecvAny(ack,this->group());
            if(!ack || !tmp){ // if there is no message or the message is empty
                continue;
            }
            cout<<this->getRank()<<" receive message from " << tmp->source()<<endl;
            // put the message to the shared queue
            int i = 0;
            bool success = this->_que.tryAdd(tmp);
            while(!success && i < this->_maxTry){
                success = this->_que.tryAdd(tmp);
                i++;
            }
            if(!success){
                stringstream ss;
                ss << tmp->source()<<"->"<<tmp->destination()<<"message ..";
                throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
            }

        }
    }
}
