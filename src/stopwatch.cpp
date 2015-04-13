/**
 * COPYRIGHT NOTICE
 * Copyright (c) 2012, Computational Science Laboratory, Stony Brook University.
 * All rights reserved.
 *
 * @file stopwatch.cpp
 * @version 1.0
 * @author Peng Zhang <Peng.Zhang@StonyBrook.edu>
 * @date 2012-10-14
 * @brief Create a stopwatch for timing.
 *
**/
#include <ctime>
#include <cstdio>

/**
 * @brief Measure the elapsed time since last call of this routine in seconds.
 * @return Returns the elapsed time in seconds.
 */
double
elapsed_second ()
{
	static clock_t last_stop = clock();
	double sec = (double) (clock() - last_stop) / (double) CLOCKS_PER_SEC;
	last_stop = clock();
	return sec;
}