#ifndef MESSAGEGENERATOR_H
#define MESSAGEGENERATOR_H
#include "processor.h"
class messageGenerator : public Processor
{
public:
    messageGenerator(Processor* p,bool& finished);
private:
    void run();
    void getMyPrefix();
private:
    bool&                        _finished;
    std::vector<int>            _myprefix;
    int                         _prefixLen;


};

#endif // MESSAGEGENERATOR_H
