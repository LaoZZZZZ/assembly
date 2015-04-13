/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file init_bcast_group.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-11-1
 * @brief Initialize an MPI_Bcast group handler.
 * @see MTPI_Bcast_handler
 *
**/
#include <iostream>
#include <sstream>

#include "mpi.h"

using namespace std;

/**
 * @brief Create a new MPI_Bcast group handler.
 * @param [in] sz Number of nodes in the new group.
 * @param [in] nodes Pointer to an integer array which listed all nodes' MPI ranks.
 * @param [out] new_rank New MPI rank of the node in the new group.
 * @param [out] new_group New group handler.
 * @param [out] new_comm New group communicator.
 * @return True if successfully; otherwise, False.
 */
bool
init_bcast_group (int sz, int * nodes, int & new_rank, MPI_Group & new_group,  MPI_Comm & new_comm)
{
	int new_size;
	MPI_Group group_world;

	if (MPI_Comm_group (MPI_COMM_WORLD, &group_world) != MPI_SUCCESS) {
		cerr << "Error occures in " << __FILE__ << ":" << __LINE__ << endl;
		return false;
	}

	if (MPI_Group_incl (group_world, sz, nodes, &new_group)  != MPI_SUCCESS) {
		cerr << "Error occures in " << __FILE__ << ":" << __LINE__ << endl;
		return false;
	}

	if (MPI_Group_size (new_group, &new_size)  != MPI_SUCCESS) {
		cerr << "Error occures in " << __FILE__ << ":" << __LINE__ << endl;
		return false;
	}

	if (sz != new_size) {  /** test if the size of a new group is as expected. */
		cerr << "Fatal error: Expected new group size = " << sz << " but got " << new_size;
		return false;
	}

	if (MPI_Group_rank (new_group, &new_rank) != MPI_SUCCESS) {
		cerr << "Error occures in " << __FILE__ << ":" << __LINE__ << endl;
		return false;
	}
	
	if (MPI_Comm_create (MPI_COMM_WORLD, new_group, &new_comm)  != MPI_SUCCESS) {
		cerr << "Error occures in " << __FILE__ << ":" << __LINE__ << endl;
		return false;
	}

	return true;
}


/**
 * @brief Initiate an empty MPI Bcast group.
 */
bool
init_self_bcast_group ()
{
	MPI_Comm newcomm;
	if (MPI_Comm_create (MPI_COMM_WORLD, MPI_GROUP_EMPTY, &newcomm) == MPI_SUCCESS)
		return true;
	return false;
}
