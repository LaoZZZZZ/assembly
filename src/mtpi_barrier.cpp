#include "mtpi_common.h"

void
MTPI_Barrier ()
{
	MPI_Barrier( MPI_COMM_WORLD );
}