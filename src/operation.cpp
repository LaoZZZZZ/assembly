#include "operation.h"

Operation::Operation()
{
}
vector<rawMessage*>  Operation::process(vector<rawMessage*>& msgs){
    vector<rawMessage*> returnmessage;

    for(rawMessage* m: msgs){
        rawMessage* tmp = new rawMessage(m->destination(),m->source(),m->message(),m->tag());
	returnmessage.push_back(tmp);
	delete m;
      
   }
    return returnmessage;
}
