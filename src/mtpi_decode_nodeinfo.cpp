/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_decode_nodeinfo.cpp
 * @version 1.0
  * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-12
 * @brief Decode an integer array of size 10 into a NodeInfo.
 *
**/
#include "mtpi_common.h"

void
MTPI_Decode_NodeInfo(
					 NodeInfo& info,
					 int * sendbuf)
{
	info.self_rank_id = sendbuf[0];
	info.system_size = sendbuf[1];
	info.mgr_rank_id = sendbuf[2];
	info.info_rank_id  = sendbuf[3];
	info.ip_address[0] = sendbuf[4];
	info.ip_address[1] = sendbuf[5];
	info.ip_address[2] = sendbuf[6];
	info.ip_address[3] = sendbuf[7];
	info.role = (NodeRole) sendbuf[8];
	info.status = (NodeStatus) sendbuf[9];

#if defined DETAILED_DEBUG
	std::stringstream ss;
	ss << "decoding a NodeInfo"
		<< ", rank = " << info.self_rank_id
		<< ", size = " << info.system_size
		<< ", mgr = " << info.mgr_rank_id
		<< ", info = " << info.info_rank_id
		<< ", ip = " << info.ip_address[0]
	    << "." << info.ip_address[1]
		<< "." << info.ip_address[2]
		<< "." << info.ip_address[3]
		<< ", role = " << noderole_int2str( info.role )
		<< ", status = " << nodestatus_int2str( info.status );
	MTPI_Write_Info( DEBUG_INFO, ss );
#endif
}