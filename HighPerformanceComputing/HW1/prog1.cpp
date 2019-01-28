#include <mpi.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
using namespace std;

int dboard(int N)
{
	//Throw darts at dartboard
	//Generate random numbers for X and Y coordinates
	//check whether dart lands in square
	return M;
}


int main(int argc, char *argv[])
{
	
	int N = atoi(argv[1]);
	int R = atoi(argv[2]);

	// set up MPI
	MPI_Init(&argc, &argv);

	// get communicator size and my rank
	MPI_Comm comm = MPI_COMM_WORLD;
	int p, rank;
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &rank);

	/* code */
	int x = 0;
	if(rank==0) x = N; 
	MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < R; ++i)
	{
		dboard(x)

	}

	if(rank==0)
	{
		// compute average
	}

	// finalize MPI
	MPI_Finalize();
	
	return 0;
}