/*
 * Convert an enum NodeRole to a char[]
 */
#include "mtpi_common.h"

const char *
noderole_int2str( NodeRole role )
{
	switch ( role ) {
		case SCHD_MGR :
			return "SchdMgr  ";
		case NOT_DECIDED :
			return "Undecided";
		case TASK_MGR :
			return "TaskMgr  ";
		case DATA_MGR :
			return "DataMgr  ";
		case INFO_MGR : 
			return "INfoMgr  ";
		case ENGINE : 
			return "Engine   ";
		default:
			std::stringstream ss("");
			ss << "Error in using NodeRole (val=" << role << ")";
			MTPI_Write_Info( ERR_INFO, ss );
	}
	return "Unresolved";
}