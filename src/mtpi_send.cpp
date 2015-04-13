/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_send.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-10
 * @brief Send a variable-sized message to a specified node. Blocking send.
 *
**/
#include "mtpi_common.h"
#include <sstream>

#define MAX_MESSAGE_PRINT_SIZE 100

using std::stringstream;


/**
 * @brief Send a string to a specified node. Blocking send.
 * @param [in] str A string that stores the message.
 * @param [in] destination Destination of the message.
 * @param [in] tag Task tag of this message.
 * @return Returns the execution status, success or failure.
 * @see MTPI_Send
 */
MTPI_Returns
MTPI_Send_String (
					 string & str,
					 int dest,
					 MTPI_Task_Tag tag)
{
	Message new_msg( str.c_str(), self_node_info.self_rank_id, dest, tag );
	return MTPI_Send( new_msg );
}


/**
 * @brief Send a CHAR array to a specified node. Blocking send.
 * @param [in] char_array CHAR array that is to be sent.
 * @param [in] destination Destination of the message.
 * @param [in] tag Task tag of this message.
 * @return Returns the execution status, success or failure.
 * @see MTPI_Send
 */
MTPI_Returns
MTPI_Send_Char_Array (
					 const char * char_array,
					 int dest,
					 MTPI_Task_Tag tag)
{
	Message new_msg( char_array, self_node_info.self_rank_id, dest, tag );
	return MTPI_Send( new_msg );
}


/**
 * @brief Send a variable-sized message to a specified node. Blocking send.
 * @param [in] new_msg Reference to Message at which the to-send data is stored.
 * @return Returns the sending status, success or failure.
 */
MTPI_Returns
MTPI_Send(
		  Message& new_msg
		  )
{
	std::stringstream ss;
	ACK_Message ack;

#if defined DETAILED_TIMING
	elapsed_second ();
#endif

#if defined DETAILED_DEBUG
	ss << "is to send a message of size " << new_msg.size()
		<< " from node " << new_msg.src()
		<< " to node " << new_msg.dest() 
		<< " with " << tasktag_int2str( new_msg.tag() );
	MTPI_Write_Info( DEBUG_INFO, ss );

	if( new_msg.size() < MAX_MESSAGE_PRINT_SIZE ) {
		ss << "is to send the data [" << new_msg.data() << "]"
			<< " from node " << new_msg.src()
			<< " to node " << new_msg.dest() 
			<< " with " << tasktag_int2str( new_msg.tag() );
		MTPI_Write_Info( DEBUG_INFO, ss );
	}
#endif

	/**
	 * Step 1: Initiate and send an ACK message to the destination.
	 */
	ack.Init( new_msg );
	ack.send( new_msg.dest() );

#if defined DETAILED_DEBUG
	ss << "sent an ACK of noticing a coming message of size " << ack.next_msg_size()
		<< " from node " << new_msg.src()
		<< " to node " << new_msg.dest()
		<< " with " << tasktag_int2str( ack.this_msg_tag() );
	MTPI_Write_Info( DEBUG_INFO, ss);
#endif

	/**
	 * Step 2: Receive an ACK message from the destination.
	 * We only check the Task Tag of a return ACK message.
	 * If the tag is MTPI_Task_Tag::INCMG_MSG, proceed to send next message;
	 * otherwise, an error message tag is given.
	 */
	ack.recv( new_msg.dest(), INCMG_MSG );

#if defined DETAILED_DEBUG
	ss << "received an ACK message from node " << ack.this_msg_src()
		<< " with " << tasktag_int2str( ack.this_msg_tag() );
	MTPI_Write_Info( DEBUG_INFO, ss);
#endif

	switch( ack.this_msg_tag() ) {
		case INCMG_MSG:
			break;

		default:
			ss << "received an ACK message with " << tasktag_int2str( ack.this_msg_tag() )
				<< " so the message sending is corrupted.";
			MTPI_Write_Info( ERR_INFO, ss);
			return MTPI_FAILURE;
	}


	/**
	 * Step 3: Send the variable-sized new message.
	 */
#if defined DETAILED_DEBUG
	ss << "is sending the message of size " << new_msg.size()
		<< " to node " << new_msg.dest()
		<< " with " << tasktag_int2str( new_msg.tag() );
	MTPI_Write_Info( DEBUG_INFO, ss);
#endif

#if defined DETAILED_TIMING
	ss << "sent a message of size " << new_msg.size()
		<< " to node " << new_msg.dest() << " with "
		<< tasktag_int2str( new_msg.tag() );
	ss << " in " << std::setprecision (6) << elapsed_second () << " seconds";
	MTPI_Write_Info( DEBUG_INFO, ss);
#endif

	return new_msg.send();
}
