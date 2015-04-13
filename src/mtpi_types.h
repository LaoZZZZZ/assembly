/**
 * @file mtpi_types.h
 * @Brief This files defines all data types used in MTPI.
 */
#ifndef _MTPI_TYPES_H
#define _MTPI_TYPES_H
#include <map>
#include <string>
#include "ppseq_defs.h"
#include "mpi.h"
using namespace std;

/**
 * @brief File format
 */
typedef enum SeqFileFormat {
	UNKNOWN_FILEFORMAT = 0,
	FASTQ_FILE,
	FASTA_FILE,
	UNPAIRED_FASTQ, /** bundle a FASTA sequence and quality data for unpaired reads */
	UNPAIRED_FASTA,
	UNPAIRED_FASTA_SEQ, /** a FASTA sequence data for unpaired reads */
	UNPAIRED_FASTA_QUA, /** a FASTA sequence quality data for unpaired reads */
	PAIRED_FASTQ,  /** bundle a FASTA sequence and quality data for paired reads */
	PAIRED_FASTA,
	PAIRED_FASTA_SEQ, /** a FASTA sequence data for paired reads */
	PAIRED_FASTA_QUA, /** a FASTA sequence quality data for paired reads */
}SeqFileFormat;

/**
 * @brief Role of a node
 */
typedef enum NodeRole {
	NOT_DECIDED = 0, ///< Role is not decided yet.
	SCHD_MGR,             ///< Scheduler manager
	INFO_MGR,              ///< 
	TASK_MGR,
	DATA_MGR,
	ENGINE
}NodeRole;


/* Status of a node */
typedef enum NodeStatus {
	UNKNOWN = 0,
	FREE,                       ///< A node is free.
	BUSY,                       ///< A node is busy.
	KILLED                    ///< A node is terminated or killed.
}NodeStatus;

typedef enum comMode{
    BLOCKING = 0,
    NONBLOCKING
}comMode;

typedef enum MTPI_Task_Tag{

    UNKNOWN_TAG = MPI_ANY_TAG,
    INTERNAL_TAG,
    GLOBAL_TAG,
    TERMINATE,
	ACK_YES,
	ACK_NO,
	ADD_SCHD_BWTCFG,       /// Add scheduler BWT configuration information
	ADD_SCHD_ALGNCFG,      /// Add scheduler ALGN configuration information
	WRITE_INFO,            ///< Write infomation in InfoMgr
	JOIN_BCAST_GROUP,      ///< Join/Build a broadcast group associated with a task manager
	INIT_BCAST_GROUP,      ///< Initiate the broadcasat group
	TMGR_BCAST,            ///< Task manager is to broadcast
	ERR_MSG,               ///< An unknown error occurs.
	ERR_NOT_ENOUGH_MEM,    ///< An error due to not enough memory
	ADD_ENGINE,            ///< Add engines to a task manager
	APPEND_ENGINE,         ///< Append engine to a task manager
	ADD_DATAFILEITEM,      ///< Add an DataFileItem to a task manager
	ASSIGN_DATAMGR,        ///< Assign a data manager rank to a node
	TEST,                  ///< Test if a node is alive and free
	INCMG_MSG,             ///< Notice the destination of an incoming message
	ADD_GENOME,            ///< Add a genome information
	ADD_ALGN_OPTIONS,      ///< Add global options to engines from tmgr
	ADD_GENOME_INDEX,      ///< Add a genome index information
	CLEAN_GENOME,          ///< Clean up a genome information
	ADD_READS,             ///< Add short reads information
	CLEAN_READS,              ///< Clean up short reads information
	ADD_GENOME_FILE_HEADER,   ///< Add a genome file header from tmgr to dmgr
	ADD_ZIP_BWT_RESULT,       ///< Add a BWT result from engine to dmgr
	ZIP_BWT,              ///< Use the BWT algorithm to compress the genome data
	ALIGN_BOWTIE,         ///< Emulate the BowTie to map short reads to genome
	ALGN_ADD_DMGRS,       ///< Add dmgrs to engine from tmgr for ALGN
	ADD_ALGN_RESULT,      ///< Add a ALGN result from engine to dmgr
	JOB_FINISHED,         ///< Job finished and status = FREE
	JOB_NOT_FINISHED,     ///< Job not finished yet
}MTPI_Task_Tag;


/**
 @brief MTPI return values.
 */
typedef enum MTPI_Returns {
	MTPI_SUCCESS = 0,    ///< Success
	MTPI_FAILURE             ///< Failure
}MTPI_Returns;


/**
 * @brief Output information types.
 */
typedef enum MTPI_Info_Msg_Type {
	UNKNOWN_MSG_TYPE = 0,
	STANDARD_INFO,
	DEBUG_INFO,
	ERR_INFO,
	WARN_INFO,
	TIME_INFO,
	DUMP_ALL
}MTPI_Info_Msg_Type;


/* Output device types */
typedef enum MTPI_Output_Device {
	UNKNOWN_DEVICE = 0,
	STANDARD_OUTPUT,
	STANDARD_ERROR_OUTPUT,
	INFOMATION_MANAGER,
	REGULAR_FILE,
	NO_OUTPUT
}MTPI_Output_Device;


/**
 * @brief A single NodeInfo for nodal basic information
 */
typedef struct NodeInfo {
	int self_rank_id;     ///< Rank ID of self
	int system_size;      ///< Total number of processes
	int mgr_rank_id;      ///< Rank ID of Manager
	int info_rank_id;     ///< Rank ID of InfoMgr
	int ip_address[4];    ///< 4-field IP address
	NodeRole role;        ///< Role of the node
	NodeStatus status;    ///< Status of the node

private:
	string _encode_data;

	/**
	 * @brief Initiate NodeInfo with default values.
	 */
public:
	NodeInfo () : self_rank_id(-1),
		system_size(-1),
		mgr_rank_id(G_SCHEDULER_RANK_ID),
		info_rank_id(-1),
		role(NOT_DECIDED),
		status(FREE) {}

public:
	/**
	 * @brief Encoded NodeInfo into a string.
	 * @return Pointer to the string.
	 */
	const char * encode () {
		char buffer [81];
		sprintf (buffer, "%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
			self_rank_id, system_size, mgr_rank_id, info_rank_id,
			ip_address[0], ip_address[1], ip_address[2], ip_address[3],
			(int) role, (int) status);
		buffer [80] = '\0';
		this->_encode_data.assign (buffer);
		return  this->_encode_data.c_str();
	}


	/**
	 * @breif Decode a string into NodeInfo.
	 * @param [in] encoded_string Pointer to a string.
	 */
	void decode (const char * encoded_string) {
		int t1, t2;
		sscanf (encoded_string, "%8d%8d%8d%8d%8d%8d%8d%8d%8d%8d",
			&self_rank_id, &system_size, &mgr_rank_id, &info_rank_id,
			&ip_address[0], &ip_address[1], &ip_address[2], &ip_address[3],
			&t1, &t2);
		role = (NodeRole) t1;
		status = (NodeStatus) t2;
	}

}NodeInfo;


/* node maps of the system */
typedef map<int, NodeInfo> NodeMap;


#endif
