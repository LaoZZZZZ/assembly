/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_init.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-10
 * @brief Initialize the MTPI execution environment.
 *
**/
#include "mtpi_common.h"

using namespace std;

/**
 * @brief  Define a global variable - self_node_info \n
 * This is the NodeInfo of this node containing basic nodel information.
 * @see mtpi_common.h
 */
NodeInfo self_node_info;

/**
 * @brief Define the length of an ACK message, value = 6 \n
      Description of an ACK message: \n
      Position 0 - Source rank of this ACK message \n
      Position 1 - Source rank of next message \n
      Position 2 - Length of next message \n
      Position 3 - Task tag of this ACK message \n
      Position 4 - Task tag of next message \n
      Position 5 - Unused field
 **/
const unsigned int ACK_MESSAGE_LENGTH = 6;


/**
 * @brief Initialize the MTPI execution environment for SchdMgr before calling SchdMgr::assign().\n
 * Only SchdMgr has to call this routine before any communication happends.
 *
 * @param [out] system This is a pointer to NodeMap. A node map will be generated 
 and returned through this reference.
 * @param [in] schd_assign_system_func This is a function handler,  which passes the 
 method of specifying engines and managers.
 *
 * @return Returns MTPI_SUCCESS if no error; MTPI_FAILURE, otherwise.
 *
 * @note Process steps: \n
 * [1] All nodes generated its own NodeInfo and sent it to SchdMgr.\n
 * [2] SchdMgr gathered all NodeInfo for generating a NodeInfo map.\n
 * [3] Assign NodeRole and engines at SchdMgr.\n
 * [4] Send back such assignment plan to all nodes.
 *
 * @see MTPI_Init_Schd_Mgr_Af_Assign MTPI_Finalize MTPI_Build MTPI_Init_Non_Schd_Mgr
 **/
MTPI_Returns
MTPI_Init_Schd_Mgr_Bf_Assign (NodeMap * system)
{
	stringstream ss;
	int length_of_nodeinfo = 10;
	int * sendbuf = NULL, * recvbuf = NULL;
	NodeMap::iterator it;

	/*!
	 * - Initiate the MPI execution platform: get its own rank and the system size.
	 * - Get IP addresss.
	 * - Only SchdMgr specifies its role as SCHD_MGR
	 */
	get_ip_address(self_node_info.ip_address);
	self_node_info.role = SCHD_MGR;

	/*!
	 * - Gather NodeInfo from all nodes at SchdMgr
	 */
	sendbuf = new int[length_of_nodeinfo];
	MTPI_Encode_NodeInfo(self_node_info, sendbuf);
	recvbuf = new int[self_node_info.system_size * length_of_nodeinfo];

	MPI_Gather(
		(void *) sendbuf, length_of_nodeinfo, MPI_INT,
		(void *) recvbuf, length_of_nodeinfo, MPI_INT,
		G_SCHEDULER_RANK_ID, MPI_COMM_WORLD);


	for(int id=0; id < self_node_info.system_size; id++) {
		NodeInfo * new_nodeinfo = new NodeInfo;
		MTPI_Decode_NodeInfo(*new_nodeinfo, &recvbuf[id*length_of_nodeinfo]);
		(*system)[id] = *new_nodeinfo;
	}
	
	ss << system->size() << " nodes registered in SchdMgr " << self_node_info.self_rank_id;
	MTPI_Write_Info(STANDARD_INFO, ss);


#if defined DETAILED_DEBUG
	int i;
	for(it=system->begin(), i=0; it!=system->end(); it++) {
		ss << "RegItem " << i++ << " : node " << it->first
			<< ", rank = " << it->second.self_rank_id << ", ip =" << it->second.ip_address[0]
		<< "." << it->second.ip_address[1] << "." << it->second.ip_address[2]
		<< "." << it->second.ip_address[3] << ", role = " << noderole_int2str( it->second.role )
			<< ", stat = " << nodestatus_int2str( it->second.status );
		MTPI_Write_Info(DEBUG_INFO, ss);
	}
#endif

		/*!
		 * - Assign node roles to the computing nodes.
		 */
	/* Free memory */
	if(sendbuf != NULL) 
		delete[] sendbuf;

	if(recvbuf != NULL )  
		delete[] recvbuf;

	return MTPI_SUCCESS;
}


/**
 * @brief Initialize the MTPI execution environment for SchdMgr after calling SchdMgr::assign().\n
 * Only SchdMgr has to call this routine before any communication happends.
 *
 * @param [out] system This is a pointer to NodeMap. A node map will be generated 
 and returned through this reference.
 * @param [in] schd_assign_system_func This is a function handler,  which passes the 
 method of specifying engines and managers.
 *
 * @return Returns MTPI_SUCCESS if no error; MTPI_FAILURE, otherwise.
 *
 * @note Process steps: \n
 * [1] All nodes generated its own NodeInfo and sent it to SchdMgr.\n
 * [2] SchdMgr gathered all NodeInfo for generating a NodeInfo map.\n
 * [3] Assign NodeRole and engines at SchdMgr.\n
 * [4] Send back such assignment plan to all nodes.
 *
 * @see MTPI_Init_Schd_Mgr_Bf_Assign MTPI_Finalize MTPI_Build MTPI_Init_Non_Schd_Mgr
 **/
MTPI_Returns
MTPI_Init_Schd_Mgr_Af_Assign (NodeMap * system)
{
	stringstream ss;
	int length_of_nodeinfo = 10;
	int * sendbuf = NULL, * recvbuf = NULL;
	NodeMap::iterator it;

	/*!
	 * - Gather NodeInfo from all nodes at SchdMgr
	 */
	sendbuf = new int[length_of_nodeinfo];
	MTPI_Encode_NodeInfo(self_node_info, sendbuf);
	recvbuf = new int[self_node_info.system_size * length_of_nodeinfo];

	ss << "SchdMgr assigned: "
		<< noderole_count( system, SCHD_MGR ) << " SchdMgr, "
		<< noderole_count( system, INFO_MGR ) << " InfoMgr, "
		<< noderole_count( system, TASK_MGR ) << " TaskMgr, "
		<< noderole_count( system, DATA_MGR ) << " DataMgr, "
		<< noderole_count( system, ENGINE ) << " Engines, "
		<< noderole_count( system, NOT_DECIDED ) << " Not Decided.";
	MTPI_Write_Info(STANDARD_INFO, ss);

	/** Encode the NodeInfo array at SchdMgr */
	for(it=system->begin(); it!=system->end(); it++)
		MTPI_Encode_NodeInfo( it->second, &recvbuf[(it->first) * length_of_nodeinfo]);

	/** Scatter new NodeInfo to all nodes */
	MPI_Scatter(
		(void *)recvbuf, length_of_nodeinfo, MPI_INT,
		(void *)sendbuf, length_of_nodeinfo, MPI_INT,
		G_SCHEDULER_RANK_ID, MPI_COMM_WORLD);

	/** Decode the new NodeInfo at all nodes */
	MTPI_Decode_NodeInfo(self_node_info, sendbuf);

	/* Free memory */
	if(sendbuf != NULL) 
		delete[] sendbuf;

	if(recvbuf != NULL )  
		delete[] recvbuf;

	ss << "MTPI initialized successfully.";
	MTPI_Write_Info(STANDARD_INFO, ss);
	MTPI_Write_Info(DUMP_ALL, ss);

	return MTPI_SUCCESS;
}


	/**
 * @brief Initialize the MTPI execution environment for non-SchdMgr nodes.\n
 * All nodes except for SchdMgr have to call this routine before any communication happends.
 *
 * @return Returns MTPI_SUCCESS if no error; MTPI_FAILURE, otherwise.
 *
 * @note Process steps: \n
 * [1] All nodes generated its own NodeInfo and sent it to SchdMgr.\n
 * [2] SchdMgr gathered all NodeInfo for generating a NodeInfo map.\n
 * [3] Assign NodeRole and engines at SchdMgr.\n
 * [4] Send back such assignment plan to all nodes.
 *
 * @see MTPI_Finalize MTPI_Build MTPI_Init_Non_Schd_Mgr
 **/
MTPI_Returns
MTPI_Init_Non_Schd_Mgr ()
{
	stringstream ss;
	int length_of_nodeinfo = 10;
	int * sendbuf = NULL, * recvbuf = NULL;
	NodeMap::iterator it;

	/*!
	 * - Initiate the MPI execution platform: get its own rank and the system size.
	 * - Get IP addresss.
	 * - Only SchdMgr specifies its role as SCHD_MGR
	 */
	get_ip_address(self_node_info.ip_address);

	/*!
	 * - Gather NodeInfo from all nodes at SchdMgr
	 */
	sendbuf = new int[length_of_nodeinfo];
	MTPI_Encode_NodeInfo(self_node_info, sendbuf);

	MPI_Gather(
		(void *) sendbuf, length_of_nodeinfo, MPI_INT,
		(void *) recvbuf, length_of_nodeinfo, MPI_INT,
		G_SCHEDULER_RANK_ID, MPI_COMM_WORLD);

	/** Scatter new NodeInfo to all nodes */
	MPI_Scatter(
		(void *)recvbuf, length_of_nodeinfo, MPI_INT,
		(void *)sendbuf, length_of_nodeinfo, MPI_INT,
		G_SCHEDULER_RANK_ID, MPI_COMM_WORLD);

	/** Decode the new NodeInfo at all nodes */
	MTPI_Decode_NodeInfo(self_node_info, sendbuf);

	/* Free memory */
	if(sendbuf != NULL)  delete[] sendbuf;
	if(recvbuf != NULL )  delete[] recvbuf;

	return MTPI_SUCCESS;
}
