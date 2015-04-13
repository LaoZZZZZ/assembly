/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_write_info.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-12
 * @brief Write any information to an InfoMgr or standard output when InfoMgr is not initiated yet.
 *
**/
#include "mtpi_common.h"

using std::string;
using std::stringstream;

/**
 * @brief Max local buffer size, in bytes.
 */
// const int MAX_WRT_BUFFER_SIZE = 4096;
const int MAX_WRT_BUFFER_SIZE = 0;

void dump_info(MTPI_Output_Device, stringstream&);

/**
 * @brief Write any information to an InfoMgr or standard output.
 * @param [in] msg_type Message type
 * @param [in] msg Message content
 */
void
MTPI_Write_Info(MTPI_Info_Msg_Type msg_type,
				stringstream& msg)
{
	bool keep_running = true;

	static stringstream output_buffer;
        string ts;

        time_stamp( ts );

	/**
	 * Append a new message header to the end of a output buffer.
	 */
	switch( msg_type ) {
		case DEBUG_INFO :
			output_buffer << "DEBG: ";
			break;

		case ERR_INFO :
			output_buffer << "ERROR: ";
			keep_running = false;
			break;

		case STANDARD_INFO :
			output_buffer << "INFO: ";
			break;

		case WARN_INFO :
			output_buffer << "WARN: ";
			break;

		case TIME_INFO :
			output_buffer << "TIME: ";
			break;

		case DUMP_ALL :
			break;

		default:
			return;
	}
	
	/**
	 * Append the new message to the end of a output buffer. 
	 */
	if( msg_type != DUMP_ALL )
		output_buffer << "[" << ts.c_str()
                << "|node " << self_node_info.self_rank_id << "] "
		<< msg.str().c_str() << std::endl;

	/**
	 * A write operation is issued if the buffer size is over a pre-defined size.
	 */
	if(output_buffer.str().size() >= MAX_WRT_BUFFER_SIZE)
		dump_info( STANDARD_OUTPUT, output_buffer );

	/**
	 * Clean up the message buffer.
	 */
	msg.str("");

	if (!keep_running)
		exit (2);
}


void dump_info(
			   MTPI_Output_Device device,
			   stringstream& ss)
{
	switch( device ) {

		case STANDARD_OUTPUT :

		default:
			if( ss.str().size() > 0 ) 
				cout << ss.str().c_str();
			cout.flush();
	}

	ss.str(""); // Clear the output buffer.
}
