/*
 * Finalize the MTPI environment.
 */
#include "mtpi_common.h"

using std::stringstream;

int
MTPI_Finalize (void)
{
	stringstream ss;
	/*
	 * Dump all of remaning messages to the information writer.
	 */
	ss << "is shut down normally.";
	MTPI_Write_Info(STANDARD_INFO, ss);

	if( IS_SCHDMGR ) {
		stringstream ss;
		ss << "shutting down the MTPI communication platform.";
		MTPI_Write_Info(STANDARD_INFO, ss);
	}

        ss.str("");
	MTPI_Write_Info(DUMP_ALL, ss);

	MPI_Barrier (MPI_COMM_WORLD);
	MPI_Finalize();
	return MPI_SUCCESS;
}
