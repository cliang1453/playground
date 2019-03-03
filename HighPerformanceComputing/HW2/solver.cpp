#include "solver.h"


/*************************** DECLARE YOUR HELPER FUNCTIONS HERE ************************/

// Check if it is valid to put Q at board[curr_row][curr_col]
bool is_valid(std::vector<int> curr, int curr_row, int curr_col, unsigned int k);

// Start from root node to right most node at level k (the initialization of partial solution)
std::vector<int> search_step(unsigned int n, unsigned int k);

// Start from current node at level k (current partial solution) to next node at level k (next partial solution) in in-order-traversal order
bool search_partial( unsigned int n, unsigned int k, std::vector<int>& curr, int col);

// Explore all leaf nodes at level n (full solutions) starting from the current node at level k (current partial solution)
void search_full(std::vector<std::vector<unsigned int> >& sols, std::vector<int>& curr, unsigned int n, unsigned int col);

/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns) {

	std::vector<int> curr(n, -1);
	search_full(all_solns, curr, n, 0);
}

void nqueen_master(	unsigned int n,
					unsigned int k,
					std::vector<std::vector<unsigned int> >& all_solns) {


	unsigned int num_procs;
	unsigned int num_sol;
	MPI_Status stat;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	bool is_found = false;

	/******************* STEP 1: Send one partial solution to each worker ********************/
	// Initialize the first partial solution
	std::vector<int> curr = search_step(n, k);
	for (int i = 0; i < num_procs; ++i)
	{
		// create a partial solution
		if (i>0)
			is_found = search_partial(n, k, curr, k-1);
		
		if (!is_found) break;
		// send partial solution to a worker (vector<int>)
		std::vector<int> partial_sol = curr;
		MPI_Send( partial_sol[0], k, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD);
	}


	/******************* STEP 2: Send partial solutions to workers as they respond ********************/
	while(is_found)
	{
		// receive completed work from a worker processor (unsigned int[][])
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		MPI_Get_count(&stat, MPI_UNSIGNED, &num_sol);

		if (num_sol != MPI_UNDEFINED)
		{
			unsigned int** sol_list = new unsigned int*[num_sol];
			for(int i = 0; i < num_sol; ++i) 
				sol_list[i] = new unsigned int[n];
		}
		
		MPI_Recv( &(sol_list[0][0]), num_sol*n, MPI_UNSIGNED, stat.SOURCE, stat.TAG, MPI_COMM_WORLD, &stat);
	    
		// store the received sol_list (unsigned int[][]) in all_solns (vector<vector<unsigned int>>)
		for (int i = 0; i < num_sol; ++i){
			for (int j = 0; j < n; ++j) 
				all_solns[i].push_back(sol_list[i][j]);
		}
		delete sol_list;

		// create a partial solution 
		is_found = search_partial(n, k, curr, k-1);

		// Break when no more partial solutions exist
		if (!is_found) break;

		// send partial solution to the word that responded
		std::vector<int> partial_sol = curr;
		MPI_Send( partial_sol[0], k, MPI_INT, stat.MPI_SOURCE, stat.TAG, MPI_COMM_WORLD);

	}

	/********************** STEP 3: Terminate **************************/
	// if no new partial solution found, we breaked from previous steps (either 1 or 2)
	// wait to receive all not yet collected sols from workers
	MPI_Barrier(MPI_COMM_WORLD);

	while(true){

		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
		MPI_Get_count(&stat, MPI_UNSIGNED, &num_sol);

		if (num_sol == MPI_UNDEFINED)
			break;
		
		unsigned int** sol_list = new unsigned int*[num_sol];
		for(int i = 0; i < num_sol; ++i) 
			sol_list[i] = new unsigned int[n];

		MPI_Recv( &(sol_list[0][0]), num_sol*n, MPI_UNSIGNED, stat.SOURCE, stat.TAG, MPI_COMM_WORLD, &stat);

		for (int i = 0; i < num_sol; ++i){
			for (int j = 0; j < n; ++j) 
				all_solns[i].push_back(sol_list[i][j]);
		}

		delete sol_list;
	}
		
	// Send termination signals to each worker processor
	std::vector<int> partial_sol(k, -1);
	MPI_Bcast( partial_sol[0], k, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);

}

void nqueen_worker(	unsigned int n,
					unsigned int k) {



	// TODO: Implement this function

	// Following is a general high level layout that you can follow (you are not obligated to design your solution in this manner. This is provided just for your ease).

	MPI_Status stat;
	std::vector<int> sol(k, 0);

	while(true) {
		
		MPI_Recv(&sol, k, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

		if(sol[0] == -1)
			return;

		sol.resize(n, -1);
		std::vector<std::vector<unsigned int> > sols;
		search_full(sols, sol, n, k);

		// convert 2D unsigned int vector to 2D unsigned int array
		unsigned int** sol_list = new unsigned int*[sols.size()];
		for(int i = 0; i < sols.size(); ++i) {
			sol_list[i] = new unsigned int[n];
			for (int j = 0; j < n; ++j)
				sol_list[i][j] = sols[i][j];
		}

		// send 2D unsigned int array
		MPI_Send(&(sol_list[0][0]), sol.size()*n, MPI_UNSIGNED, 0, stat.TAG, MPI_COMM_WORLD);
	}
}



/*************************** DEFINE YOUR HELPER FUNCTIONS HERE ************************/

// Check if it is valid to put Q at board[curr_row][curr_col]
bool is_valid(std::vector<int> curr, int curr_row, int curr_col, unsigned int k){
	
	for (int col = 0; col < curr_col; ++col)
	{
		if (curr_row == curr[col]) 
			return false;
		int row = curr[col];

		if (curr_row + curr_col - row < k){
			if (curr[curr_row + curr_col - row] == row) 
				return false;}

		if (-curr_row + curr_col + row >= 0 && -curr_row + curr_col + row < k){
			if (curr[-curr_row + curr_col + row] == row) 
				return false;}
	}
		

	return true;
}

// Start from root node to right most node at level k (the initialization of partial solution)
std::vector<int> search_step(unsigned int n, unsigned int k){
	
	std::vector<int> curr(k, -1);
	
	for (int col = 0; col < k; ++col)
	{
		for (int row = 0; row < n; ++row)
		{
			if (!is_valid(curr, row, col, k)) 
				continue;
			curr[col] = row;
			break;
		}
	}
	
	return curr;
}

// Start from current node at level k (current partial solution) to next node at level k (next partial solution) in in-order-traversal order
bool search_partial( unsigned int n, unsigned int k, std::vector<int>& curr, int col){

	if (col < 0) 
		return false;

	for (int row = curr[col]+1; row < n; ++row)
	{
		if (!is_valid(curr, row, col, k)) 
			continue;

		curr[col] = row;
		if (col == k-1) 
			return true;
		else 
			return search_partial(n, k, curr, col+1);
	}

	curr[col] = -1;
	return search_partial(n, k, curr, col-1);
}

void search_full(std::vector<std::vector<unsigned int> >& sols,
				 std::vector<int>& curr, unsigned int n, unsigned int col)
{
	if (col == n){
		std::vector<int> new_sol = curr;
		sols.push_back(new_sol);
		return;
	}

	for (int row = 0; row < n; ++row)
	{
		if (!is_valid(curr, row, col, n)) 
			continue;

		curr[col] = row;
		search_full(sols, curr, n, col+1);
		curr[col] = -1;
	}
}





