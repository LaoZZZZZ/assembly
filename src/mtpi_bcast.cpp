/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_bcast.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-11-1
 * @brief Broadcast a message amongst all engines on a task manager.
 *
**/
#include "mtpi_common.h"


/**
 * @brief Send a variable-sized message to a specified node. Blocking send.
 * @param [in] new_msg Reference to Message at which the to-send data is stored.
 * @return Returns the sending status, success or failure.
 */
MTPI_Returns
MTPI_Bcast(
		  Message& new_msg,
		  MTPI_Bcast_handler * handler
		  )
{
	stringstream ss;
	int msg_size = -1;
	int msg_tag = (int) UNKNOWN_TAG;

	/** broadcast the message size. */
	if (self_node_info.self_rank_id == handler->tmgr_rank()) {
		msg_size = new_msg.size();
		msg_tag = (int) new_msg.tag();
#if defined DETAILED_DEBUG
		ss << "is to broadcast a message of size " << msg_size <<
			" with tag " << tasktag_int2str ((MTPI_Task_Tag)msg_tag);
		MTPI_Write_Info (DEBUG_INFO, ss);
#endif
	}

	MPI_Bcast (&msg_size, 1, MPI_INT, 0, handler->comm());
	MPI_Bcast (&msg_tag, 1, MPI_INT, 0, handler->comm());

#if defined DETAILED_DEBUG
	if (self_node_info.self_rank_id != handler->tmgr_rank()) {
		ss << "is to receive a broadcast message of size " << msg_size <<
			" with " << tasktag_int2str ((MTPI_Task_Tag)msg_tag);
		MTPI_Write_Info (DEBUG_INFO, ss);
	}
#endif

	return new_msg.bcast (handler->tmgr_rank(), self_node_info.self_rank_id, msg_size,
		(MTPI_Task_Tag) msg_tag, handler->comm());
}