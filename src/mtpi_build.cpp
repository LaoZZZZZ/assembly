/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file mtpi_build.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-16
 * @brief Build the MPI-based communication environment.
 *
**/
#include "mtpi_common.h"


/**
 * @brief Build the MPI-based communication environment.
 * @param [in] argc Pointer to the number of arguments.
 * @param [in] argv Pointer to the argument vector.
 * @return MPI rank of the current process.
 */
int
MTPI_Build (int *argc, char *** argv) {
	MPI_Init(argc, argv);
	MPI_Comm_rank (MPI_COMM_WORLD, &self_node_info.self_rank_id);
	MPI_Comm_size (MPI_COMM_WORLD, &self_node_info.system_size);
	return self_node_info.self_rank_id;
}
