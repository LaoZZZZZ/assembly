/**
 * @file mtpi_ack_message.h
 * @brief Definition of ACK message of fixed length.
 */
#ifndef _MTPI_ACK_MESSAGE_H
#define _MTPI_ACK_MESSAGE_H
/**
 * @file mtpi_message.h
 * @brief A generic message for communication between nodes.
 */
#include "mtpi_types.h"
#include "mtpi_message.h"

/*
 * Declearations of global variables
 */
extern NodeInfo self_node_info;
extern const unsigned int ACK_MESSAGE_LENGTH;


/**
 * @brief Class of ACK_Message
 */
class ACK_Message {
private:
	int * msg;

public:
	/**
	 * Constructure
	 */
	ACK_Message (Message& msg) {
		ACK_Message();
		this->Init( msg );
	}

	ACK_Message () {
		this->msg = new int[ACK_MESSAGE_LENGTH];
	}

	/**
	 * Destructure
	 */
	~ACK_Message () {
		if( msg != NULL )
			delete[] msg;
	}

public:
	/**
	 * Initialize an ACK message.
	 */
	int * Init( Message& msg ) {
		return this->Init( self_node_info.self_rank_id,
			msg.src(), msg.size(), INCMG_MSG,
			msg.tag());
	}

	/**
	 * Initialize an ACK message.
	 * Returns the pointer to the ACK message.
	 */
	int * Init( int this_msg_source,
		int next_msg_source,
		int next_msg_length,
		MTPI_Task_Tag this_msg_tag,
		MTPI_Task_Tag next_msg_tag) {
			return this->Init( this_msg_source,
				next_msg_source, next_msg_length,
				this_msg_tag, next_msg_tag, 0);
	}

	/**
	 * Initialize an ACK message.
	 * Returns the pointer to the ACK message.
	 */
	int * Init( int this_msg_source,
		int next_msg_source,
		int next_msg_length,
		MTPI_Task_Tag this_msg_tag,
		MTPI_Task_Tag next_msg_tag,
		int opt_field) {
			msg[0] = this_msg_source;
			msg[1] = next_msg_source;
			msg[2] = next_msg_length;
			msg[3] = (int) this_msg_tag;
			msg[4] = (int) next_msg_tag;
			msg[5] = opt_field;
			return this->data();
	}

	/**
	 * Returns a pointer to the ACK message.
	 */
	int * data () {
		return this->msg;
	}

	/**
	 * Size of an ACK message.
	 */
	size_t size() {
		return ACK_MESSAGE_LENGTH;
	}

	/**
	 * Send an ACK message to a node.
	 */
	MTPI_Returns send (int dest) {
		if( MPI_Send( (void *) this->msg, ACK_MESSAGE_LENGTH, MPI_INT,
			dest, (int) INCMG_MSG,
			MPI_COMM_WORLD) == MPI_SUCCESS )
			return MTPI_SUCCESS;
		return MTPI_FAILURE;
	}

	/**
	 * Receive an ACK message from a given node.
	 */
	MTPI_Returns recv (int src, MTPI_Task_Tag tag) {
		MPI_Status status;
		try {
		if( MPI_Recv( (void *) this->msg, ACK_MESSAGE_LENGTH, MPI_INT,
			src, (int) tag, MPI_COMM_WORLD, &status) == MPI_SUCCESS )
			return MTPI_SUCCESS;
		} catch (...) {
			cerr << "An error occured in MPI_Recv" << endl;
		}
		return MTPI_FAILURE;
	}

	MTPI_Returns recv (int src) {
		return this->recv(src, INCMG_MSG);
	}

	/**
	 * Receive an ACK message from any source with any tag.
	 */
	MTPI_Returns recv_any () {
		MPI_Status status;
		if( MPI_Recv( (void *) this->msg, ACK_MESSAGE_LENGTH, MPI_INT,
			MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status)
			== MPI_SUCCESS )
			return MTPI_SUCCESS;
		return MTPI_FAILURE;
	}

	/**
	 * Utility member functions to manipulate a single position.
	 */
	int this_msg_src () {
		return this->msg[0];
	}

	int this_msg_src (int new_src) {
		this->msg[0] = new_src;
		return this_msg_src();
	}

	int next_msg_src() { 
		return this->msg[1]; 
	}

	int next_msg_src(int new_src) {
		this->msg[1] = new_src;
		return this->next_msg_src(); 
	}

	int next_msg_size() {
		return this->msg[2];
	}

	int next_msg_size(int new_size) {
		this->msg[2] = new_size;
		return this->next_msg_size();
	}

	MTPI_Task_Tag this_msg_tag() {
		return (MTPI_Task_Tag) this->msg[3];
	}

	MTPI_Task_Tag this_msg_tag(MTPI_Task_Tag new_tag) {
		this->msg[3] = (int) new_tag;
		return this->this_msg_tag();
	}

	MTPI_Task_Tag next_msg_tag() {
		return (MTPI_Task_Tag) this->msg[4];
	}

	MTPI_Task_Tag next_msg_tag(MTPI_Task_Tag new_tag) {
		this->msg[4] = (int) new_tag;
		return this->next_msg_tag();
	}

	int opt_field () {
		return this->msg[5];
	}

	int opt_field (int new_val) {
		this->msg[5] = new_val;
		return this->opt_field();
	}

};


#endif
