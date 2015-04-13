#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <stdexcept>
#include <sstream>
#include <string>
#include "rawmessage.h"
using std::string;
using std::stringstream;
/**
  * An exception class that could be used when the maximum times has been tried before success
*/
class maximumTryError : public std::exception{
public:
    explicit maximumTryError(int rank,int maximumtry,const string& message){
        stringstream ss;
        ss << rank <<" tried "<<maximumtry<< " times :: " << message<<endl;
        this->message.assign(ss.str());
    }
    maximumTryError(const maximumTryError& e){
        this->message.assign(e.what());
    }
    maximumTryError& operator=(const maximumTryError& e){
        this->message.assign(e.what());
        return *this;
    }
    const char* what() const throw(){
        return this->message.c_str();
    }
private:
    std::string                         message;
};

/**
  * Send exception, incurred when something wrong when sending the message
*/

class sendError : public std::exception{
public:
    explicit sendError(rawMessage* msg){
        this->init(msg);
    }
    sendError(const sendError& e){
        this->message.assign(e.what());
    }
    sendError& operator=(const sendError& e){
        this->message.assign(e.what());
        return *this;
    }
    const char* what() const throw(){
        return this->message.c_str();
    }
private:
    void init(rawMessage* msg){
        stringstream ss;
        ss <<"Something wrong when sending the message :: ";
        ss << "from "<<msg->source()<<" to "<<msg->destination();
        ss << " The message tag is "<< msg->tag()<< " and the message size is " << msg->size()<<endl;


        message.assign(ss.str());

    }
private:
    std::string                          message;

};

/**
  * Receive exception, incurred when something wrong when sending the message
*/

class receiveError : public std::exception{
public:
    explicit receiveError(rawMessage* msg){
        this->init(msg);
    }
    receiveError(const receiveError& e){
        this->message.assign(e.what());
    }
    receiveError& operator=(const receiveError& e){
        this->message.assign(e.what());
        return *this;
    }
    const char* what() const throw(){
        return this->message.c_str();
    }
private:
    void init(rawMessage* msg){
        stringstream ss;
        ss <<"Process " << msg->source() << " can not receive message with tag "<< msg->tag()
          <<" from process " << msg->destination()<<"; Something wrong when waiting for the message!"<<endl;
        message.assign(ss.str());

    }
private:
    std::string                          message;

};
#endif // EXCEPTIONS_H
