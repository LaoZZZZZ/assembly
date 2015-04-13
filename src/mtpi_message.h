#ifndef _MTPI_MESSAGE_H
#define _MTPI_MESSAGE_H
/**
 * @file mtpi_message.h
 * @brief A generic message for communication between nodes.
 */
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include "mpi.h"
#include "mtpi_types.h"


/**
 * @brief A generic message class for inter-nodal communication.
 */
class Message {
private:
	/**
	 * @brief Data Section.
	 */
	string msg;           ///< Message string
	int source;           ///< Source of the message
	int destination;     ///< Destination of the message
	MTPI_Task_Tag task_tag;  ///< Task tag of the message

public:
	/**
	 * @brief Construct.
	 */
	Message (void) { 
		Message("");
	}

	/**
	 * @brief Construct.
	 */
	Message (const char * data) {
		Message ( data, -1, -1, UNKNOWN_TAG );
	}

	/**
	 * @brief Construct.
	 */
	Message (const char * data, int s, int d, MTPI_Task_Tag t) {
		Init( data, s, d, t);
	}
    const string& message()const{return this->msg;}
	/**
	 * @brief Destructure
	 */
	~Message(void) {
		this->clear();
	}
    Message(const std::string& msg,int s,int d,MTPI_Task_Tag t = UNKNOWN_TAG){
        this->Init(msg,s,d,t);
    }

	/**
	 * @brief Initiate the message/task data
	 */
	void Init(const char * data, int s, int d, MTPI_Task_Tag t) {
		this->clear();
		this->assign( data );
		this->src( s );
		this->dest( d );
		this->tag( t );
	}

	/**
	 * @brief Initiate the message/task data
	 */
	void Init(const string& data, int s, int d, MTPI_Task_Tag t) {
		this->clear();
		this->assign( data );
		this->src( s );
		this->dest( d );
		this->tag( t );
#if defined DETAILED_DEBUG
		if (data.size() != msg.size ()) {
			cerr << "incorrect message size (" << msg.size() << ") "
			   << "does not match input message size " << data.size()
			   << endl;
			exit (2);
		}
#endif
	}

	/**
	 * @brief Return source of the message.
	 */
	int src () {
		return this->source;
	}

	/**
	 * @brief Set the source.
	 * @return Return source of the message.
	 */
	int src (int new_src) {
		this->source = new_src;
		return this->src();
	}

	/**
	 * @brief Return destination of the message.
	 */
	int dest () {
		return this->destination;
	}

	/**
	 * @brief Set the destination.
	 * @return Return destination of the message.
	 */
	int dest (int new_dest) {
		this->destination = new_dest;
		return this->dest();
	}

	/**
	 * @brief Return tag of the message.
	 */
	MTPI_Task_Tag tag () {
		return this->task_tag;
	}

	/**
	 * @brief Set the task tag.
	 * @return Return task tag of the message.
	 */
	MTPI_Task_Tag tag(MTPI_Task_Tag new_tag) {
		this->task_tag = new_tag;
		return this->tag();
	}

	/**
	 * @brief Assign data to the message.
	 * @return Returns size of the current message.
	 */
	size_t assign (const char * s ) {
		msg.assign( s );
		return size();
	}

	/**
	 * @brief Assign data to the message.
	 * @return Returns size of the current message.
	 */
	size_t assign (const char * s, size_t n) {
		this->msg.assign(s, n);
		return this->size();
	}

	/**
	 * @brief Assign data to the message.
	 * @return Returns size of the current message.
	 */
	size_t assign (const string& str ) {
		this->msg.assign( str );
		return this->size();
	}

	/**
	 * @brief Assign data to the message.
	 * @return Returns size of the current message.
	 */
	size_t assign (const string& str, size_t pos, size_t n ) {
		this->msg.assign( str, pos, n );
		return this->size();
	}

	/**
	 * @brief Clean up the message.
	 * @return Returns the size of the message content, should be 0.
	 */
	int clear() {
		this->msg.clear();
		this->source = -1;
		this->destination = -1;
		this->task_tag = UNKNOWN_TAG;
		if (this->size () > 0) {
			printf ("ERROR: cannot clean a message.\n");
			exit (1);
		}
		return this->size();
	}

	/**
	 * @brief Returns a count of the number of characters in the message.
	 */
	size_t size () { 
		return this->msg.size();
	}

	/**
	 * @brief Returns a pointer to an array of characters with the same content as the message.
	 */
	const char * data() {
		return this->msg.data();
	}

	/**
         * @return Pointer to msg (msg container)
         */
	string * msg_ptr () { return &msg; }

	/**
	 * @brief Returns the maximum number of characters that the message can hold.
	 */
	size_t max_size () {
		return this->msg.max_size();
	}

	/**
	 * @brief Resize the message content to n charactors.
	 */
	MTPI_Returns resize (size_t sz) {
		clear();
		msg.resize(sz, '\0');
		if( size() == sz )
			return MTPI_SUCCESS;
		return MTPI_FAILURE;
	}

	/**
	 * @brief Send this message to a given node.
	 */
	MTPI_Returns send () {
		if( MPI_Send( (void *) this->msg.c_str(), this->size(), MPI_CHAR,
			this->dest(), (int) this->tag(), MPI_COMM_WORLD) == MPI_SUCCESS )
			return MTPI_SUCCESS;
		return MTPI_FAILURE;
	}

	/**
	* @brief Receive a message to the local node.
	* @param [in] s Source rank
	* @param [in] d Destination rank
	* @param [in] sz Size of the message
	* @param [in] t Task tag
	* @return If the operation is successful
	*/
	MTPI_Returns recv (int s, int d, int sz, MTPI_Task_Tag t) {
		MPI_Status status;
		this->clear();
		if( this->resize(sz) == MTPI_FAILURE)
			return MTPI_FAILURE;
		try {
		if( MPI_Recv( (void *) msg.c_str(), sz, MPI_CHAR,
			s, t, MPI_COMM_WORLD, &status) != MPI_SUCCESS)
			return MTPI_FAILURE;
		} catch (...) {
			cerr << "An error occured in MPI_Recv" << endl;
			cerr.flush ();
			return MTPI_FAILURE;
		}
		this->source = s;
		this->destination = d;
		this->task_tag = t;
		return MTPI_SUCCESS;
	}

	/**
	 * @brief Broadcast a message from source to all nodes.
	 * @param [in] s Root rank of the broadcast message
	 * @param [in] d Destination of the boradcast message, i.e., its own rank.
	 * @param [in] sz Size of the broadcast message
	 * @param [in] t Task tag
	 * @param [in] handler Pointer to the broadcast handler
	 * @return If the operation is successful
	 */
	MTPI_Returns bcast (int s, int d, int sz, MTPI_Task_Tag t, MPI_Comm comm) {
		if (s != d) { /** non-root nodes */
			this->clear();
			if (this->resize(sz) == MTPI_FAILURE)
				return MTPI_FAILURE;
			if (MPI_Bcast ((void *)msg.c_str(), sz, MPI_CHAR, 0, comm) != MPI_SUCCESS)
				return MTPI_FAILURE;
			this->source = s;
			this->destination = d;
			this->task_tag = t;
		}
		else {  /** root node: 0 */
			if (MPI_Bcast ((void *)msg.c_str(), sz, MPI_CHAR, 0, comm) != MPI_SUCCESS)
				return MTPI_FAILURE;
		}

		return MTPI_SUCCESS;
	}

};

#endif
