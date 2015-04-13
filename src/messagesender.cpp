#include "messagesender.h"
void messageSender::run(){
    if(this->_mode == NONBLOCKING){
        while(!this->_finished || !this->_que.empty()){
            rawMessage* tmp = NULL;
            bool succ = this->_que.try_remove(tmp);
            if(succ){  // if got one message and the message is not empty
                assert(tmp);
                // message for myself
                if(tmp->destination() == this->getRank()){
                    bool success =false;
                    int tries = 0;
                    while(!success){
                        success = this->_inque.tryAdd(tmp);
                        tries++;
                        if(tries > this->_maxTry){
                            stringstream ss;
                            ss << tmp->source()<<"->"<<tmp->destination()<<": message ...";
                            throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
                        }
                    }
                }else{
                    // nonblocking sending
                    MPI_Request req = MPI_P2P::ISend(tmp,this->group());
                    MPI_Status status;
                    if(MPI_SUCCESS != MPI_Wait(&req,&status)){
                        throw receiveError(tmp);
                    }
                    /*
                    if(!status){
                        int i = 0;
                        while(i < this->_maxTry && !status){
                            status = MPI_P2P::ISend(tmp,this->group());
                            i++;
                        }
                        if(!status){
                            stringstream ss;
                            ss << tmp->source()<<"->"<<tmp->destination()<<": message ...";
                            throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
                        }
                    }*/
                    delete tmp;
                    tmp = NULL;
                }
            }else
                this->yield();
        }
    }
    else{
        while(!this->_finished || !this->_que.empty()){
            rawMessage* tmp = NULL;
            bool succ = this->_que.try_remove(tmp);
            if(succ){  // if got one message and the message is not empty
                assert(tmp);
                // message for myself
                if(tmp->destination() == this->getRank()){
                    bool success =false;
                    int tries = 0;
                    while(!success){
                        success = this->_inque.tryAdd(tmp);
                        tries++;
                        if(tries > this->_maxTry){
                            stringstream ss;
                            ss << tmp->source()<<"->"<<tmp->destination()<<": message ...";
                            throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
                        }
                    }
                }else{
                    // blocking sending
                    bool status = MPI_P2P::Send(tmp,this->group());
                    if(!status){
                        int i = 0;
                        while(i < this->_maxTry && !status){
                            status = MPI_P2P::Send(tmp,this->group());
                            i++;
                        }
                        if(!status){
                            stringstream ss;
                            ss << tmp->source()<<"->"<<tmp->destination()<<": message ...";
                            throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
                        }
                    }
                    delete tmp;
                    tmp = NULL;
                }
            }else
                this->yield();
        }
    }
}
