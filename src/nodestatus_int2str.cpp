/*
 * Convert an enum NodeStatus to a const char string
 */
#include "mtpi_common.h"


const char *
nodestatus_int2str (NodeStatus status)
{
	switch (status) {
		case FREE :
			return "Free   ";

		case BUSY :
			return "Busy   ";

		case UNKNOWN :
			return "Unknown";

		default :
			std::stringstream ss("");
			ss << "Error in using NodeStatus (status=" << status << ")";
			MTPI_Write_Info( ERR_INFO, ss );
	}
	return "Unresolved";
}