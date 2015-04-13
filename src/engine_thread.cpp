#include "engine_thread.h"

engine_thread::engine_thread(std::shared_ptr<Parameter> p):Processor(p)
{
}

void engine_thread::drive(){
    bool finished = false;
    Processor* mg1 = new messageGenerator(this,finished);
    mg1->start();

    //mg
    /////////////////////////////////////////////////
    int groupsize = this->groupSize();
    int me = this->getRank();
    int totalreceived = 0;

    while(true){
        if(totalreceived >= groupsize - 1)
            break;
        else{
            bool succ = false;
            rawMessage* rev = this->getMessage(succ);
            if(succ){
                totalreceived++;
                assert(rev && rev->tag() == INTERNAL_TAG);
            }
            //cout<<totalreceived<<endl;
        }

    }
    cout<<"all received"<<endl;
    finished=true;
    mg1->join();
    delete mg1;
    rawMessage* tmp = NULL;
   tmp = this->Barrrior(new rawMessage(me,0," ",INTERNAL_TAG));
   finished = false;
   cout<<"second round"<<endl;
   mg1 = new messageGenerator(this,finished);
   mg1->start();
   totalreceived = 0;

    while(true){
        if(totalreceived >= groupsize - 1)
            break;
        else{
            bool succ = false;
            rawMessage* rev = this->getMessage(succ);
            if(succ){
                totalreceived++;
                assert(rev && rev->tag() == INTERNAL_TAG);
            }
        }

    }
    finished= true;
    mg1->join();
    delete mg1;
    cout<<me << " start to barrier"<<endl;
    tmp = this->Barrrior(new rawMessage(me,0,"",TERMINATE));
    assert(tmp && tmp->tag() == TERMINATE);
}

