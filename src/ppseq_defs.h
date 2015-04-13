/*
 * This file defines all globals used for PPSEQ.
 */
#ifndef _PPSEQ_DEFINES_H
#define _PPSEQ_DEFINES_H

/* Default Rank ID for Scheduler Manager */
const static int G_SCHEDULER_RANK_ID = 0;
const static long G_MAX_MESSAGE_SIZE = (10*1024*1024);

/**
 * Macros
 */
#define INT_WIDTH 8  /** width of an integer in encoding and decoding */
#define LONG_INT_WIDTH 16  /** width of a long integer in encoding and decoding */
#define FLOAT_WIDTH 16  /** width of a float in encoding and decoding */
#define DOUBLE_WIDTH 16  /** width of a double in encoding and decoding */

#endif