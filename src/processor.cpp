#include "processor.h"

std::shared_ptr<netComm> Processor::_network(NULL);
std::unordered_map<int,int> Processor::_rankLookUpTable;
Processor::Processor(std::shared_ptr<Parameter> params):mtpiThread(params->globalParameters()->comGroup()),_params(params)
{
    this->init();
}
void Processor::init(){
    if(!Processor::_network.get())
        Processor::initGlobal(this->_params);
}

bool Processor::addMessage(rawMessage* m){
    return Processor::_network->addMessage(m);

}
rawMessage* Processor::getMessage(bool& succ){
    return Processor::_network->getMessage(succ);
}
/**
  * barrior implementation
*/
rawMessage* Processor::barriorImp(rawMessage* msg){
    //make sure the message is valid
    assert(msg);
    // pause all sending and receiving threads
    Processor::_network->stop();
    cout<<this->getRank()<<" successfully pause send and receive thread "<<endl;
    // synchronize with master node
    // current we assume the 0 rank process is the master node

    //assert(msg->destination() == 0);

    bool succ = MPI_P2P::ISend(msg,this->group());
    cout<<this->getRank() <<" sent message to master and start to wait"<<endl;
    succ = false;
    rawMessage* confirmMessage = NULL;
    confirmMessage = MPI_P2P::Recv(msg->destination(),succ,this->group());
    if(!succ || !confirmMessage){
        rawMessage* expectMsg = new rawMessage(msg->destination(),msg->source(),"",msg->tag());
        throw receiveError(expectMsg);
    }
    cout<<this->getRank()<<" received confirm message  from master"<<endl;
    // restart the sending and receiving threads
   if(confirmMessage->tag() != TERMINATE){
        Processor::_network->resume();
   }
    return confirmMessage;
}
