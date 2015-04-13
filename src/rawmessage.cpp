#include "rawmessage.h"

rawMessage::rawMessage(int src,int dest,
                       const std::string& msg,MTPI_Task_Tag tag):
                       _src(src),_dest(dest),_msg(msg),_tag(tag)
{
}
