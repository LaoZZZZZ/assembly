/**
 * @file time_stamp.cpp
 */
#include <time.h>
#include <string>

using namespace std;


/**
 * @brief Return a time stamp.
 * @param [out] str String containing a human-readable date and time.
 */
void
time_stamp ( string & str )
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [15];

  time( &rawtime );
  timeinfo = localtime ( &rawtime );

  strftime(buffer, 15, "%m-%d-%X", timeinfo);
  str.assign( buffer );
}
