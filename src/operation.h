#ifndef OPERATION_H
#define OPERATION_H
#include "rawmessage.h"
#include <vector>
using std::vector;
/**
  * Apply an corresponding Given an array of messages,
  *
*/
class Operation
{
public:
    Operation();
    vector<rawMessage*>  process(vector<rawMessage*>& msgs);

};

#endif // OPERATION_H
