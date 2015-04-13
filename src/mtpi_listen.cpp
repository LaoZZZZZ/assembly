/**
 * @file mtpi_listen.cpp
 * @brief Listen to the network for any message.
 * MPI blocking Receive.
 */
#include "mtpi_common.h"

#define MAX_MESSAGE_PRINT_SIZE 100

using std::iostream;
using std::stringstream;

Message* MTPI_Listen(MTPI_Task_Tag tag, MTPI_Returns& status){
    stringstream ss;
    ACK_Message ack;
    Message* new_msg = new Message();
#if defined DETAILED_DEBUG
    ss << "is listening";
    MTPI_Write_Info( DEBUG_INFO, ss );
#endif

    /**
     * Wait for any ACK message from any source with any tag.
     */
    ack.recv_any();

#if defined DETAILED_DEBUG
    ss << "received an ACK message with " << tasktag_int2str( ack.this_msg_tag() )
        << " from node " << ack.this_msg_src()
        << " noticing of a coming message of size " << ack.next_msg_size()
        << " with " << tasktag_int2str( ack.next_msg_tag() );
    MTPI_Write_Info( DEBUG_INFO, ss );
#endif

    /**
     * Update the source of a return ACK message.
     */
    ack.this_msg_src( self_node_info.self_rank_id );

    /**
     * Return the ACK message to the sender.
     */
    ack.send( ack.next_msg_src() );

#if defined DETAILED_DEBUG
    ss << "waitng for a message of size " << ack.next_msg_size()
        << " from node " << ack.next_msg_src()
        << " with " << tasktag_int2str( ack.next_msg_tag() );
    MTPI_Write_Info( DEBUG_INFO, ss );
#endif

    /**
     * Wait for the next Message and receive the message into new_msg.
     */
    status = new_msg->recv( ack.next_msg_src(), self_node_info.self_rank_id,
        ack.next_msg_size(), ack.next_msg_tag() );

#if defined DETAILED_DEBUG
    if( status == MTPI_SUCCESS ) {
        ss << "received a message of size " << new_msg.size()
            << " with " << tasktag_int2str( new_msg.tag() )
            << " from node " << new_msg.src();
        MTPI_Write_Info( DEBUG_INFO, ss );
        if( new_msg.size() < MAX_MESSAGE_PRINT_SIZE ) {
            ss << "received the message data [" << new_msg.data()
                << "] from node " << new_msg.src() << " with "
                << tasktag_int2str( new_msg.tag() );
            MTPI_Write_Info( DEBUG_INFO, ss );
        }
    }
#endif

    return new_msg;
}


Message* MTPI_Listen(MTPI_Returns& status){
    stringstream ss;
    ACK_Message ack;
    Message* new_msg = new Message();
#if defined DETAILED_DEBUG
    ss << "is listening";
    MTPI_Write_Info( DEBUG_INFO, ss );
#endif

    /**
     * Wait for any ACK message from any source with any tag.
     */
    ack.recv_any ();

#if defined DETAILED_DEBUG
    ss << "received an ACK message with " << tasktag_int2str( ack.this_msg_tag() )
        << " from node " << ack.this_msg_src()
        << " noticing of a coming message of size " << ack.next_msg_size()
        << " with " << tasktag_int2str( ack.next_msg_tag() );
    MTPI_Write_Info( DEBUG_INFO, ss );
#endif

    /**
     * Update the source of a return ACK message.
     */
    ack.this_msg_src( self_node_info.self_rank_id );

    /**
     * Return the ACK message to the sender.
     */
    ack.send( ack.next_msg_src() );

#if defined DETAILED_DEBUG
    ss << "waitng for a message of size " << ack.next_msg_size()
        << " from node " << ack.next_msg_src()
        << " with " << tasktag_int2str( ack.next_msg_tag() );
    MTPI_Write_Info( DEBUG_INFO, ss );
#endif

    /**
     * Wait for the next Message and receive the message into new_msg.
     */
    status = new_msg->recv( ack.next_msg_src(), self_node_info.self_rank_id,
        ack.next_msg_size(), ack.next_msg_tag() );

#if defined DETAILED_DEBUG
    if( status == MTPI_SUCCESS ) {
        ss << "received a message of size " << new_msg.size()
            << " with " << tasktag_int2str( new_msg.tag() )
            << " from node " << new_msg.src();
        MTPI_Write_Info( DEBUG_INFO, ss );
        if( new_msg.size() < MAX_MESSAGE_PRINT_SIZE ) {
            ss << "received the message data [" << new_msg.data()
                << "] from node " << new_msg.src() << " with "
                << tasktag_int2str( new_msg.tag() );
            MTPI_Write_Info( DEBUG_INFO, ss );
        }
    }
#endif

    return new_msg;
}

MTPI_Returns
MTPI_Listen(
			Message& new_msg
			)
{
	stringstream ss;
	ACK_Message ack;
	MTPI_Returns res;

#if defined DETAILED_DEBUG
	ss << "is listening";
	MTPI_Write_Info( DEBUG_INFO, ss );
#endif

	/**
	 * Wait for any ACK message from any source with any tag.
	 */
	ack.recv_any ();

#if defined DETAILED_DEBUG
	ss << "received an ACK message with " << tasktag_int2str( ack.this_msg_tag() )
		<< " from node " << ack.this_msg_src()
		<< " noticing of a coming message of size " << ack.next_msg_size()
		<< " with " << tasktag_int2str( ack.next_msg_tag() );
	MTPI_Write_Info( DEBUG_INFO, ss );
#endif

	/**
	 * Update the source of a return ACK message.
	 */
	ack.this_msg_src( self_node_info.self_rank_id );

	/**
	 * Return the ACK message to the sender.
	 */
	ack.send( ack.next_msg_src() );

#if defined DETAILED_DEBUG
	ss << "waitng for a message of size " << ack.next_msg_size()
		<< " from node " << ack.next_msg_src()
		<< " with " << tasktag_int2str( ack.next_msg_tag() );
	MTPI_Write_Info( DEBUG_INFO, ss );
#endif

	/**
	 * Wait for the next Message and receive the message into new_msg.
	 */
	res = new_msg.recv( ack.next_msg_src(), self_node_info.self_rank_id,
		ack.next_msg_size(), ack.next_msg_tag() );

#if defined DETAILED_DEBUG
	if( res == MTPI_SUCCESS ) {
		ss << "received a message of size " << new_msg.size()
			<< " with " << tasktag_int2str( new_msg.tag() )
			<< " from node " << new_msg.src();
		MTPI_Write_Info( DEBUG_INFO, ss );
		if( new_msg.size() < MAX_MESSAGE_PRINT_SIZE ) {
			ss << "received the message data [" << new_msg.data()
				<< "] from node " << new_msg.src() << " with "
				<< tasktag_int2str( new_msg.tag() );
			MTPI_Write_Info( DEBUG_INFO, ss );
		}
	}
#endif
	
	return res;
}
