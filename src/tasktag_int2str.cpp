/*
 * Convert an enum MTPI_Task_Tag to a const char string.
 */
#include "mtpi_common.h"


const char *
tasktag_int2str( MTPI_Task_Tag tag )
{
	switch ( tag ) {
		case ADD_GENOME :
			return "ADD_GENOME";
		case ADD_GENOME_INDEX :
			return "ADD_GENOME_INDEX";
		case ADD_ALGN_OPTIONS:
			return "ADD_ALGN_OPTIONS";
		case ADD_SCHD_ALGNCFG:
			return "ADD_SCHD_ALGNCFG";
		case ALGN_ADD_DMGRS:
			return "ALGN_ADD_DMGRS";
		case ADD_ALGN_RESULT:
			return "ADD_ALGN_RESULT";
		case ADD_READS :
			return "ADD_READS";
		case ADD_ENGINE :
			return "ADD_ENGINE";
		case ADD_DATAFILEITEM :
			return "ADD_DATAFILEITEM";
		case CLEAN_GENOME :
			return "CLEAN_GENOME";
		case CLEAN_READS :
			return "CLEAN_READS";
		case ALIGN_BOWTIE :
			return "ALIGN_BOWTIE";
		case ZIP_BWT :
			return "ZIP_BWT";
		case INCMG_MSG :
			return "INCMG_MSG";
		case ERR_MSG :
			return "ERR_MSG";
		case ERR_NOT_ENOUGH_MEM :
			return "ERR_NOT_ENOUGH_MEM";
		case TERMINATE :
			return "TERMINATE";
		case TEST :
			return "TEST";
		case UNKNOWN_TAG :
			return "UNKNOWN_TAG";
		case WRITE_INFO :
			return "WRITE_INFO";
		case JOIN_BCAST_GROUP :
			return "JOIN_BCAST_GROUP";
		case INIT_BCAST_GROUP :
			return "INIT_BCAST_GROUP";
		case ASSIGN_DATAMGR :
			return "ASSIGN_DATAMGR";
		case ADD_GENOME_FILE_HEADER :
			return "ADD_GENOME_FILE_HEADER";
		case TMGR_BCAST :
			return "TMGR_BCAST";
		case ADD_ZIP_BWT_RESULT :
			return "ADD_ZIP_BWT_RESULT";
		case JOB_FINISHED :
			return "JOB_FINISHED";
		case JOB_NOT_FINISHED :
			return "JOB_NOT_FINISHED";
		default:
			std::stringstream ss("");
			ss << "Error in using MTPI_Task_Tag (tag=" << tag << ")";
			MTPI_Write_Info(ERR_INFO, ss);
	}
	return "Unresolved";
}
