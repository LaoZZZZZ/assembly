#include "messagegenerator.h"

messageGenerator::messageGenerator(Processor* p,bool& finished):Processor(p),_finished(finished),
                                                                _prefixLen(p->params()->globalParameters()->prefixLength())
{
    this->getMyPrefix();
}

void messageGenerator::run(){
    int groupsize = this->groupSize();
    int me = this->getRank();
    int i = 0;
    while(!_finished || i < groupsize){

        i++;
        if(i < groupsize){
            stringstream msg;
	    /*
            for(int i : this->_myprefix){
                msg<<i<<',';
            }
	    */
	    msg<<string(10000000,'c');
            msg.flush();
            string m(msg.str());
            rawMessage* tmp = new rawMessage(me,i,m,INTERNAL_TAG);
            if(!this->addMessage(tmp))
                 throw maximumTryError(me,this->params()->globalParameters()->maxTry(),m);
            cout<<me << " sent message to "<< i<<endl;

        }
        else{
           this->yield();
        }
    }
    cout<<"finished"<<endl;
}

void messageGenerator::getMyPrefix(){

    int me = this->getRank();
    int totalPrefix = pow(4,this->_prefixLen);
    for(int i = 0; i < totalPrefix; i++){
        if(this->getProcessID(i) == me){
            this->_myprefix.push_back(i);
        }
    }

}
