#include "netcomm.h"

netComm::netComm(MPI_Comm g,int quesize,int maxTry,comMode m,int numTh):_inque(quesize),_outque(quesize),
                                                    _finished(false),_group(g),
                                                    _maxTry(maxTry),_running(false),_mode(m),_numThread(numTh)
{
    this->resume();
}
bool netComm::addMessage(rawMessage* m){
    int tries = 0;
    bool succ = false;
    while(tries < this->_maxTry && !succ){
        succ = this->_outque.tryAdd(m);
        tries ++;
    }
    return succ;

}
rawMessage* netComm::getMessage(bool& succ){
    int tries = 0;
    rawMessage* res = NULL;
    while(tries < this->_maxTry && !succ){
        succ = this->_inque.try_remove(res);
        tries ++;
    }
    return res;
}
/**
  * pause the receive and send threads
  *
*/
void netComm::stop(){
    if(this->_running){
        this->_finished = true;
        if(this->_numThread == 2){
            if(this->_receiver.get()){
                this->_receiver->join();
            }
            if(this->_sender.get()){
                this->_sender->join();
            }
            this->_receiver.reset();
            this->_sender.reset();
        }
        else{
            assert(this->_numThread == 1);
            if(this->_sendRecver.get())
                this->_sendRecver->join();
            this->_sendRecver.reset();
        }
        this->_running = false;

    }
}
/**
  * restart the send and receive thread
  */
void netComm::resume(){
    if(!this->_running){
        this->_finished = false;
        if(this->_numThread == 2){
            this->_sender.reset(new messageSender(this->_outque,this->_inque,this->_finished,this->_maxTry,this->_group,this->_mode));
            this->_receiver.reset(new messageReceiver(this->_inque,this->_finished,this->_maxTry,this->_group,this->_mode));
            assert(this->_receiver.get());
            assert(this->_sender.get());
            this->_receiver->start();
            this->_sender->start();
        }
        else{
            this->_sendRecver.reset(new messageSenderReceiver(this->_inque,this->_outque,this->_finished,this->_maxTry,this->_group,this->_mode));
            assert(this->_sendRecver.get());
            this->_sendRecver->start();
        }
        this->_running = true;
    }
}
