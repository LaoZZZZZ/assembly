/*
 * This file is a common header file for all MTPI routines.
 */
#ifndef _MTPI_COMMON_H
#define _MTPI_COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include "mpi.h"
#include "mtpi_funcs.h"
#include "mtpi_types.h"

/**
 * @brief Declearations of global variables
 */
extern NodeInfo self_node_info;
extern const unsigned int ACK_MESSAGE_LENGTH;

/**
 * @brief Enabled/Disenabled detailed debug or timing information.
 */
// #define DETAILED_DEBUG
// #define DETAILED_TIMING


#define IS_SCHDMGR (self_node_info.self_rank_id == G_SCHEDULER_RANK_ID)
#define IS_NOT_SCHDMGR (self_node_info.self_rank_id != G_SCHEDULER_RANK_ID)

/*
 * Utility functions
 */
const char * get_ip_address( int * );
void time_stamp ( string & str );
const char * noderole_int2str (NodeRole);
const char * nodestatus_int2str (NodeStatus);
const char * tasktag_int2str (MTPI_Task_Tag);
double elapsed_second ();
int noderole_count(NodeMap *, NodeRole);
/** convert a message size to a human readable formatted string */
const char * msgsize2readable (int size, string & msg);

bool init_bcast_group (int, int *, int &, MPI_Group &,  MPI_Comm &);
bool init_self_bcast_group ();

#define MAX(a, b) ((a>b)?(a):(b))
#define MIN(a, b) ((a<b)?(a):(b))

#endif
