#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H
#include <vector>
#include <memory>
#include "rawmessage.h"
#include "mtpithread.h"
#include "operation.h"
#include "mpi_p2p.h"
#include "mtpi_exceptions.h"
using std::vector;
using std::shared_ptr;
/**
  * An abstract of global communication among the different processes
  * Basically, it gather a specific type of messages from engines
  * Then call the corresponding operation and return a list of response messageProcessor
  * Finally, it will send all message back to those engines that sent message to me
*/
class messageProcessor : public mtpiThread
{
public:
    messageProcessor(MPI_Comm group,int totalNumber);
    bool        teminate()const{return this->_terminate;}
private:
    /**
      * @brief listen and grab a specified message
    */
    void run(){
	cout<<"master start to receive message from engine"<<endl;
        while(_recieved < _totaldue){
            bool succ = MPI_P2P::listenAny();
            if(succ){
                rawMessage* tmp = MPI_P2P::IRecvAny(succ,this->group());

                if(tmp){
                    this->_terminate = (tmp->tag() == TERMINATE);
                    _recieved++;
                    _messages.push_back(tmp);
                }
            }
        }
        vector<rawMessage*> processedM = this->_opt->process(this->_messages);
        assert(processedM.size() == this->_totaldue);
	cout<<"master node start to send message back to engine!"<<endl;
        for(size_t i = 0; i < this->_totaldue;i++){
            bool succ = MPI_P2P::Send(processedM[i],this->group());
            if(!succ){
                throw sendError(processedM[i]);
            }
        }
    }
private:
    vector<rawMessage*>                     _messages;
    size_t                                  _totaldue;
    MTPI_Task_Tag                           _interestedTag;
    std::shared_ptr<Operation>              _opt;
    size_t                                  _recieved;
    bool                                    _terminate;
};

#endif // MESSAGEPROCESSOR_H
