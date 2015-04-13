#ifndef NETCOMM_H
#define NETCOMM_H
#include "lockfreequeue.h"
#include "rawmessage.h"
#include "messagesender.h"
#include "messagereceiver.h"
#include "messagesenderreceiver.h"

/**
  * @brief A simple representation of the network
  * which could send and receive packages from the network
*/
class netComm
{
public:
    /**
      * @brief
      */
    netComm(MPI_Comm,int quesize,int maxTry,comMode m,int threadNum);
    /**
     * @brief send message to network
     * @param message to be sent
     */
    bool addMessage(rawMessage* m);
    /**
      *@brief Grap one message from the network
      *@param indicator that shows whether this action is success or not
    */
    rawMessage* getMessage(bool& succ);
    /**
      *@brief pause the receive and send threads
      *
    */
    void stop();
    /**
      *@brief restart the send and receive thread
      */
    void resume();
    /**
      * check if the inque and out que is empty
      */
    bool empty(){
        return this->_inque.empty() && this->_outque.empty();
    }
    ~netComm(){
    }
private:
    lockFreeQueue<rawMessage*>                  _inque;    // message received from other processes
    lockFreeQueue<rawMessage*>                  _outque;   // message ready for sent
    std::shared_ptr<messageSender>              _sender;
    std::shared_ptr<messageReceiver>            _receiver;
    std::shared_ptr<messageSenderReceiver>      _sendRecver;
    bool                                        _finished;
    MPI_Comm                                    _group;
    int                                         _maxTry;
    bool                                        _running;
    comMode                                     _mode;
    int                                         _numThread;
};

#endif // NETCOMM_H
