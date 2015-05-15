#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "netcomm.h"
#include "mtpithread.h"
#include "parameter.h"
#include "generatetable.h"
class Processor : public mtpiThread
{
public:
    typedef std::unordered_map<int,int> LookupTable;
public:
    Processor(std::shared_ptr<Parameter> params);
    Processor(Processor* me);
    virtual ~Processor(){

    }
    bool addMessage(rawMessage* m);
    rawMessage* getMessage(bool& succ);
    rawMessage*    Barrrior(rawMessage* msg){return this->barriorImp(msg);}
    //
    virtual bool finished()const{throw 1;}
    int getProcessID(int prefix){
        assert(Processor::_rankLookUpTable.count(prefix));
        return Processor::_rankLookUpTable.at(prefix);
    }
    std::shared_ptr<Parameter>      params(){return this->_params;}
    /**
      * initialize the network configuration and look up table
    */
    static void initGlobal(std::shared_ptr<Parameter> params){
        Processor::_network.reset(new netComm(params->globalParameters()->comGroup(),
                                         params->globalParameters()->queuesize(),
                                         params->globalParameters()->maxTry(),
                                         params->globalParameters()->communicateMode(),
                                         params->globalParameters()->communicateThread()));
        if(params->globalParameters()->RandomMapping()){
            Processor::_rankLookUpTable = generateTable::randomMapping(params->globalParameters()->numberOfProcess(),
                                                         params->globalParameters()->prefixLength(),
                                                         params->globalParameters()->randomeSeed());

        }else{
            Processor::_rankLookUpTable =generateTable::fixedMapping(params->globalParameters()->numberOfProcess(),
                                                        params->globalParameters()->prefixLength());

        }
    }
private:
    virtual void drive(){throw 1;}
    void run(){
        this->_network->resume();
        this->drive();
    }
private:

    /**
      * barrior implementation
    */
    rawMessage* barriorImp(rawMessage*);
    void init();

public:
    static std::shared_ptr<netComm>                    _network;
    static LookupTable                                 _rankLookUpTable;
private:
    std::shared_ptr<Parameter>                         _params;

};

#endif // PROCESSOR_H
