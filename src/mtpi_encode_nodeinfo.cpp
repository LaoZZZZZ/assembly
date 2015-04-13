/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_encode_nodeinfo.cpp
 * @version 1.0
  * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-12
 * @brief Encode a NodeInfo into an integer array
 *
**/
#include "mtpi_common.h"


/**
 * @brief Encode NodeInfo into an integer array.
 *
 * @param [in] info A reference to the NodeInfo to be encoded.
 * @param [out] sendbuf Pointer to an integer array of size 10.
 *
 */
void
MTPI_Encode_NodeInfo(
					 NodeInfo& info,
					 int * sendbuf)
{
	sendbuf[0] = info.self_rank_id;
	sendbuf[1] = info.system_size;
	sendbuf[2] = info.mgr_rank_id;
	sendbuf[3] = info.info_rank_id;
	sendbuf[4] = info.ip_address[0];
	sendbuf[5] = info.ip_address[1];
	sendbuf[6] = info.ip_address[2];
	sendbuf[7] = info.ip_address[3];
	sendbuf[8] = (int) info.role;
	sendbuf[9] = (int) info.status;

#if defined DETAILED_DEBUG
	std::stringstream ss;
	ss << "encoding a NodeInfo"
		<< ", rank = " << sendbuf[0]
	    << ", size = " << sendbuf[1]
		<< ", mgr = " << sendbuf[2]
		<< ", info = " << sendbuf[3]
		<< ", ip = " << sendbuf[4] << "." << sendbuf[5] << "." << sendbuf[6] << "." << sendbuf[7]
		<< ", role = " << noderole_int2str( (NodeRole) sendbuf[8] )
		<< ", status = " << nodestatus_int2str( (NodeStatus) sendbuf[9] );
	MTPI_Write_Info( DEBUG_INFO, ss );
#endif
}