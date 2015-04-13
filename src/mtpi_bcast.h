/**
 * @file mtpi_bcast.h
 * @brief Header file for MTPI_Bcast
 * @note All nodes in MPI_COMM_WORLD have to create a MPI_Bcast_hanlder
 even though unuseful in some nodes.
 */
#ifndef _MTPI_BCAST_H
#define _MTPI_BCAST_H

#include "mtpi_common.h"

using namespace std;

/* essential utility routines */
bool init_bcast_group (int, int *, int &, MPI_Group &,  MPI_Comm &);
bool init_self_bcast_group ();

/* related utility routines */
void MTPI_Write_Info(MTPI_Info_Msg_Type, stringstream&);
const char * noderole_int2str( NodeRole role );


/**
 * @brief MTPI Bcast handler
 * @note This handler has to be initiated at any node involved.
 */
class MTPI_Bcast_handler {
private:
	int old_rank;  /** original MPI rank */
	int old_size;  /** size of original communication group */
	int new_rank;  /** MPI rank in the new communication group */
	int new_size;  /** size of this new communication group */
	int * ranks;  /** ranks of all nodes in this new communication group */

	MPI_Group group_world;
	MPI_Comm MTPI_GROUP_WORLD;  /** new communicator */
	MPI_Group MTPI_TMGR_GROUP;  /** new communication group */

	bool if_initiated;  /** indicate if the communicator is properly initiated */
	bool is_involved;

	string _encoded_data;  /** encoded data for messaging */

public:
	/**
	 * @brief Initiate a MTPI_Bcast_handler at Task Manager.
	 * @param [in] n Number of engines to be added to a broadcast group.
	 * @param [in] engine_ranks Pointer to a rank array of all engines to be added.
	 * @param [in] tmgr_rank MPI rank of the task manager.
	 * @note This constructor is designed for Task Manager.
	 */
	MTPI_Bcast_handler (int n, int * engine_ranks,  NodeInfo * tmgr_info) :
	  old_rank(tmgr_info->self_rank_id), old_size(tmgr_info->system_size), new_rank(0), new_size(n+1),
	  ranks(NULL), if_initiated(false), is_involved(true)
	{
		ranks = new int[new_size];
		ranks[0] = tmgr_info->self_rank_id;  /** task manager is the # 0 rank in the new group */
		for  (int i = 0; i < n; i++)  /** engines */
			ranks[i+1] = engine_ranks[i];

#if defined DETAILED_DEBUG
		stringstream ss;
		ss << "Bcast handler is created at TMgr " << ranks[0] << " with group size " << new_size;
		MTPI_Write_Info (DEBUG_INFO, ss);
#endif
	}


	/**
	  * @brief Initiate a MTPI_Bcast_handler at Engine.
	   * @param [in] msg_data Pointer to a Message data, should be an encoded data format.
	   * @param [in] engine_info Pointer to the NodeInfo of its own.
	   * @note This constructor is designed for Engine.
	   */
	MTPI_Bcast_handler (const char * msg_data,  NodeInfo * engine_info) :
	  old_rank (engine_info->self_rank_id), old_size (engine_info->system_size), new_rank (-1), new_size (-1),
	  ranks(NULL), if_initiated(false), is_involved(true)
	  {
		  stringstream ss;

		  switch ( engine_info->role ) {
				case ENGINE :
					this->decode (msg_data);
					break;

				case TASK_MGR :
					this->decode (msg_data);
					MTPI_Bcast_handler (this->new_size - 1, this->ranks, engine_info); 
					break;

				default :
					ss << "Cannot resolve the node role " << noderole_int2str (engine_info->role);
					MTPI_Write_Info (ERR_INFO, ss);
		  }

#if defined DETAILED_DEBUG
		ss << "Bcast handler is created at Engine " << engine_info->self_rank_id
			<< " with TMgr " << ranks[0] << " and group size " << new_size;
		MTPI_Write_Info (DEBUG_INFO, ss);
#endif
	  }
	    
	 /**
	  * @brief Initiate a MTPI_Bcast_handler at a non-TMgr and non-Engine node.
	   * @note This constructor is designed for a node except for TMgr or Engine.
	   */
	  MTPI_Bcast_handler () : ranks(NULL), if_initiated(true), is_involved(false) {
		  return;
	  }

	  /**
	   * Free MPI group and communicators at destruction.
	   */
	  ~MTPI_Bcast_handler ( ) {
		  if (if_initiated && is_involved) {
			  MPI_Group_free (&MTPI_TMGR_GROUP);
			  MPI_Comm_free (&MTPI_GROUP_WORLD);
		  }
		  if (ranks!=NULL) 
			  delete [] ranks;
		  this->_encoded_data.clear();
#if defined DETAILED_DEBUG
		  stringstream ss;
		  ss << "Bcast handler is destroyed";
		  MTPI_Write_Info (DEBUG_INFO, ss);
#endif
	  }


	  /** Utility functions */

	  /**
	   * @return Returns the task manager's MPI rank.
	   */
	  int tmgr_rank () {
		  if (ranks==NULL)  return -1;
		  return ranks[0];
	  }


	  /**
	   * @return Return group size
	   */
	  int group_size () {  return new_size;  }

	  /**
	   * @return Return the new group communicator
	   */
	  MPI_Comm comm () { return MTPI_GROUP_WORLD; }


	  /**
	   * @brief Initiate a MTPI_Bcast communicator.
	   * @note This has to be called as long as all ranks are known.
	   */
	  bool Init () {
		  stringstream ss;
		  if (if_initiated)  {
			  init_self_bcast_group ();  /** initiate an empty group */
			  return true;
		  }

		  if_initiated = init_bcast_group (new_size, ranks, new_rank, MTPI_TMGR_GROUP, MTPI_GROUP_WORLD);

		  if (if_initiated)
			  return true;

		  ss << "Failed in initiating the bcast group at " << __FILE__ << ":" << __LINE__;
		  MTPI_Write_Info (ERR_INFO, ss);
		  return false;
	  }


public:
	  /**
	   * Encode the ranks into a message.
	   * @returns Returns the pointer to the encoded data string.
	   * @see MTPI_Bcast_handler::decode
	   */
	  const char * encode () {
		  this->_encoded_data.clear();
		  size_t len = new_size * INT_WIDTH+ 1;
		  char * buffer = new char [len];
		  memset (buffer, '\0', len);
		  for (int i=0; i<new_size; i++)  sprintf (buffer, "%s%*d", buffer, INT_WIDTH, ranks[i]);
		  this->_encoded_data.assign (buffer);
		  delete[] buffer;
		  return this->_encoded_data.c_str();
	  }

	  /**
	   * Decode a string back to the ranks.
	   * @return Returns the number of nodes in the new communication group.
	   * @note Decode a message string, i.e., encoded data, into ranks.
	   Both ranks and new_size are initiated.
	   * @see MTPI_Bcast_handler::encode
	   */
	  int decode (const char * data) {
		  char * buffer = new char [INT_WIDTH+1];
		  memset (buffer, '\0', INT_WIDTH+1);
		  new_size = strlen (data) / INT_WIDTH;  /* number of elements in the group */
		  ranks = new int [new_size];
		  if (buffer!=NULL && ranks!=NULL) {
			  for (int i = 0; i < new_size; i++) {
				  strncpy (buffer, &(data[i*INT_WIDTH]), INT_WIDTH);
				  ranks [i] = atoi (buffer);  /* ranks of nodes in the group */
			  }
			  delete[] buffer;
		  }
		  else {
			  stringstream ss;
			  ss << "cannot allocate memory for buffer or ranks";
			  MTPI_Write_Info (ERR_INFO, ss);
		  }
		  return group_size();
	  }

	  /**
	   * Convert a serial number of a node to its rank in MPI_COMM_WORLD
	   * @param [in] sn Serial number of a node, [0, group_size - 1].
	   * @return MPI rank of the node or -1 if the serial number is out of range.
	   */
	  int sn2rank (int sn) {
		  if (sn < group_size())
			  return ranks[sn];
		  return -1;
	  }
};

#endif