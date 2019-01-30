#include <mpi.h>
#include <stdio.h>      /* printf, fgets */
#include <stdlib.h>     /* atoi */
#include <math.h>       /* cos */
#include <ios>
#include <iostream>
#include<fstream>
#include <vector>
#include <random>
#include <ctime>
using namespace std;

#define PI 3.14159265
const double r = 1000.0;

int dboard(int N, int rank)
{
	double a, theta, x, y;
	int M = 0;
	default_random_engine generator;

	for (int i = 0; i < N; ++i)
	{
		//Throw darts at dartboard
		uniform_real_distribution<double> a_dist(0.0, r*r);
		uniform_real_distribution<double> theta_dist(0.0, 360.0);
		a = a_dist(generator);  // generates number in the range 1..6 
		theta = theta_dist(generator);

		//Generate random numbers for X and Y coordinates
		x = sqrt(a) * cos(theta * PI / 180.0);
		y = sqrt(a) * sin(theta * PI / 180.0);
		
		//check whether dart lands in square
		if (abs(x) <= r/sqrt(2) && abs(y) <= r/sqrt(2))
		{
			M += 1;
		}
	}
	
	return M;
}

int data_writer(double p, int N, int R, double P, double time)
{	
	ofstream f;
	f.open("output.txt", ios_base::app | ios_base::out);
	f << "N = " <<N<<", R = "<<R<<", P = "<<p<<", PI = "<<P<<"\n";
	f << "Time = "<<time<<"\n";
	f.close();
}

int main(int argc, char *argv[])
{
	
	int N = atoi(argv[1]);
	int R = atoi(argv[2]);
	int m = 0;
	clock_t begin = clock();
	vector<int> sum(R, 0);
	vector<double> pi(R, 0);
	
	// set up MPI
	MPI_Init(&argc, &argv);

	// get communicator size and my rank
	MPI_Comm comm = MPI_COMM_WORLD;
	int p, rank;
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &rank);

	int x = 0;
	if(rank == 0) x = N/p; 
	MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
	srand (time(NULL));
	srand(rank+1);


	for (int i = 0; i < R; ++i)
	{
		m = dboard(x, rank); 

		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Reduce(&m, &sum[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if (rank == 0)
		{
			pi[i] = 2 * (double)N/sum[i];
		}
	}


	if(rank == 0)
	{
		double P = 0;
		for (int i = 0; i < R; ++i)
		{
			P += pi[i];
		}
		P = P/R;

		clock_t end = clock();
		data_writer(p, N, R, P, end-begin);
	}

	// finalize MPI
	MPI_Finalize();
	return 0;
}