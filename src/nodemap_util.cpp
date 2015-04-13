/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file nodemap_util.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-14
 * @brief Utility routines for manipulating NodeMap
 *
**/
#include "mtpi_common.h"


/**
 * @brief Find the number of nodes with a specific NodeRole.
 * @param [in] sys Pointer to a NodeMap.
 * @param [in] role Specific NodeRole
 * @return The number of nodes with the specific nodal role.
 */
int
noderole_count(
			   NodeMap * sys,
			   NodeRole role)
{
	int count = 0;
	NodeMap::iterator it;
	for( it=sys->begin(); it != sys->end(); it++ )
		if( it->second.role == role )
			count ++;
	return count;
}