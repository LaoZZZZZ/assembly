#include "messagesenderreceiver.h"

messageSenderReceiver::messageSenderReceiver(lockFreeQueue<rawMessage*>& inque,
                                            lockFreeQueue<rawMessage*>& outque,
                                            bool& finish,int maxTry,
                                            MPI_Comm group,comMode m):
                                            mtpiThread(group),
                                            _inque(inque),_outque(outque),
                                            _finished(finish),
                                            _maxTry(maxTry),_sendMode(m)
{
    this->_bufferCap = 50000000;
    assert(!_finished);
}

void messageSenderReceiver::run(){

    if(this->_sendMode == NONBLOCKING){
        while(!this->_finished || !this->_outque.empty()){
            //try to receive a message from the network
            bool ack;
            rawMessage*  tmp = NULL;
            bool succ = false;
            // non blocking receive message from the channel
            tmp = MPI_P2P::IRecvAny(ack,this->group());
            if(ack && tmp){ // if there is no message or the message is empty
                cout<<this->getRank()<<" receive message from " << tmp->source()<<endl;
                // put the message to the shared queue
                int i = 0;
                succ = this->_inque.tryAdd(tmp);
                while(!succ && i < this->_maxTry){
                    this->yield();
                    succ = this->_inque.tryAdd(tmp);
                    i++;
                }
                if(!succ){
                    stringstream ss;
                    ss << tmp->source()<<"->"<<tmp->destination()<<"message ..";
                    throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
                }
            }

            if(this->_buffered >= this->_bufferCap){
                int flag = 0;
		cout<<this->getRank()<<" the sending buffer is full"<<endl;
                size_t sz = this->_bufferedReq.size();
                assert(sz == this->_waitMessages.size());
                MPI_Request* a = new MPI_Request[sz];
                for(size_t i = 0; i < sz; i++){
                    a[i] = this->_bufferedReq[i];
                }
                int status = MPI_Testall(sz,a,&flag,MPI_STATUSES_IGNORE);
                delete []a;
                a = NULL;
                if(status == MPI_ERR_IN_STATUS){
                    throw sendError(this->_waitMessages[0]);
                }else if(flag){
                    for(size_t i = 0; i < sz; i++){
                        delete this->_waitMessages[i];
                    }
                    this->_waitMessages.clear();
                    this->_bufferedReq.clear();
                    this->_buffered = 0;
		    cout<<this->getRank()<<" cleaned the sending buffer"<<endl;
                }
            }else{
                tmp = NULL;
                // get message from the shared queue, and send it
                succ = this->_outque.try_remove(tmp);
                if(succ){  // if got one message and the message is not empty
                    assert(tmp);
                    // message for myself
                    if(tmp->destination() == this->getRank()){
                        bool success =false;
                        int tries = 0;
                        while(!success){
                            this->yield();
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
                            this->_buffered += tmp->size();
                            this->_bufferedReq.push_back(req);
                            this->_waitMessages.push_back(tmp);
                   }

                }
            }

        }
        if(this->_waitMessages.size()){
            int sz = this->_bufferedReq.size();
            MPI_Request* a = new MPI_Request[sz];
            for(int i = 0; i < sz; i++){
                a[i] = this->_bufferedReq[i];
            }
            if(MPI_SUCCESS != MPI_Waitall(sz,a,MPI_STATUSES_IGNORE)){
                throw sendError(this->_waitMessages[0]);
            }
            for(int i = 0; i < sz; i++){
                delete this->_waitMessages[i];
            }
            this->_waitMessages.clear();
            this->_bufferedReq.clear();
            this->_buffered = 0;

        }
    }else if(this->_sendMode == BLOCKING){
            while(!this->_finished || !this->_outque.empty()){
                //try to receive a message from the network
                bool ack;
                rawMessage*  tmp = NULL;
                bool succ = false;
                // non blocking receive message from the channel
                tmp = MPI_P2P::RecvAny(ack,this->group());
                if(ack && tmp){ // if there is no message or the message is empty
                    cout<<this->getRank()<<" receive message from " << tmp->source()<<endl;
                    // put the message to the shared queue
                    int i = 0;
                    succ = this->_inque.tryAdd(tmp);
                    while(!succ && i < this->_maxTry){
                        this->yield();
                        succ = this->_inque.tryAdd(tmp);
                        i++;
                    }
                    if(!succ){
                        stringstream ss;
                        ss << tmp->source()<<"->"<<tmp->destination()<<"message ..";
                        throw maximumTryError(this->getRank(),this->_maxTry,ss.str());
                    }
                }
                tmp = NULL;
                // get message from the shared queue, and send it
                succ = this->_outque.try_remove(tmp);
                if(succ){  // if got one message and the message is not empty
                    assert(tmp);
                    // message for myself
                    if(tmp->destination() == this->getRank()){
                        bool success =false;
                        int tries = 0;
                        while(!success){
                            this->yield();
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
                        bool status = MPI_P2P::Send(tmp,this->group());
                        if(!status){
                            int i = 0;
                            while(i < this->_maxTry && !status){
                                this->yield();
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
                }

            }
    }

}
