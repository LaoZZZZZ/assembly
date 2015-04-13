/*
 * Send a reference genome from one node to the other.
 *
 * Return a flag to indicate if this task is successfully done or not.
 *
 * See also: MTPI_Send
 */
#include "mtpi_common.h"

MTPI_Returns
MTPI_Send_Genome (
				  const char  * genome,
				  int engine_rank
				  )
{
	Message new_msg ( genome, self_node_info.self_rank_id, engine_rank, ADD_GENOME);
	return MTPI_Send( new_msg );
}