/*
 * This file contains all MTPI function declearations.
 */
#ifndef _MTPI_FUNCS_H
#define _MTPI_FUNCS_H

#include "mtpi_ack_message.h"
#include "mtpi_bcast.h"
#include "rawmessage.h"

/**
 * Build a MPI-based communication environment.
 */
int MTPI_Build (int *, char ***);


/**
 * Initiate an MTPI communication platform for SchdMgr and non-SchdMgr
 * before and after calling SchdMgr::assign().
 */
MTPI_Returns MTPI_Init_Schd_Mgr_Bf_Assign (NodeMap *);
MTPI_Returns MTPI_Init_Schd_Mgr_Af_Assign (NodeMap *);


MTPI_Returns MTPI_Init_Non_Schd_Mgr ();


/*
 * Encode a NodeInfo into an integer array.
 */
void
MTPI_Encode_NodeInfo(
					 NodeInfo&, 
					 int *
					 );

/*
 * Decode a NodeInfo from an integer array.
 */
void 
MTPI_Decode_NodeInfo(
					 NodeInfo&, 
					 int *
					 );

/*
 * Finalize an MTPI communication platform.
 */
int 
MTPI_Finalize(
			  void
			  );

/*
 * Send a variable-sized message to a specific node.
 */
MTPI_Returns 
MTPI_Send(
		  Message&
		  );

MTPI_Returns
MTPI_Send_String (
					 string & str,
					 int dest,
					 MTPI_Task_Tag tag);

MTPI_Returns
MTPI_Send_Char_Array (
					 const char * char_array,
					 int dest,
					 MTPI_Task_Tag tag);

/**
 * Broadcast a Message amongst all Engines from a Task Manager.
 */
MTPI_Returns
MTPI_Bcast(
		  Message& new_msg,
		  MTPI_Bcast_handler * handler
		  );


/*
 * Send a reference genome to a specific node.
 * See also: MTPI_Send
 */
MTPI_Returns
MTPI_Send_Genome (
				  const char  *,
				  int
				  );

/*
 * Send a short read to a specific node.
 * See also: MTPI_Send
 */
MTPI_Returns 
MTPI_Send_ShortReads ( 
					  const char  *,
					  int
					  );


/*
 * Listen to the network for a variable-sized message.
 */
MTPI_Returns 
MTPI_Listen(
			Message&
			);
/**
  * Listen to the network for any message
*/
Message* MTPI_Listen(MTPI_Returns& status);


/**
  * Listen to a specific type of message
*/
Message* MTPI_Listen(MTPI_Task_Tag tag, MTPI_Returns& status);
/*
 * Barrier all ranks.
 */
void MTPI_Barrier ();

/*
 * Write a system information including debug information.
 */
void MTPI_Write_Info(MTPI_Info_Msg_Type, stringstream&);


#endif
