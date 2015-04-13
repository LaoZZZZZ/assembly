#include "messageprocessor.h"

messageProcessor::messageProcessor(MPI_Comm group,
                                   int totalNumber
                                  ):mtpiThread(group),_totaldue(totalNumber),
                                                      _recieved(0),_terminate(false)
{
	this->_opt.reset(new Operation());
}
