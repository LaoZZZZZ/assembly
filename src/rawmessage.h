#ifndef MESSAGE_H
#define MESSAGE_H
#include "mtpi_message.h"
#include <string>
#include <cassert>
#include "mtpi_funcs.h"
using std::string;

/**
  * General message framework in the project
  * source,destination,message itself as well as the message tag
*/
class rawMessage
{
    ////////////////////////////////////////////////////////////////////
    ////\brief constructors,copy constructors and assign operator
    ///
public:
    rawMessage(void):_src(-1),_dest(-1),_msg(""),_tag(UNKNOWN_TAG){}
    rawMessage(int src,int dest,const std::string& msg,MTPI_Task_Tag tag);
    rawMessage(const rawMessage& cp){
        this->init(cp);
    }
    rawMessage& operator=(const rawMessage& cp){
        this->init(cp);
        return *this;
    }
    ~rawMessage(){
    }
public:
    ////////////////////////////////////////////////////////////////
    /// \brief interface
    /// \return
    ///
    int source()const{return this->_src;}
    int destination()const{return this->_dest;}
    const std::string& message()const{return this->_msg;}
    MTPI_Task_Tag tag()const{return this->_tag;}
    size_t        size()const{return this->_msg.size();}
private:
    void init(const rawMessage& cp){
        this->_src = cp.source();
        this->_dest = cp.destination();
        this->_msg.assign(cp.message());
        this->_tag = cp.tag();

    }
private:
    int                               _src;
    int                               _dest;
    std::string                       _msg;
    MTPI_Task_Tag                     _tag;
};

#endif // MESSAGE_H
