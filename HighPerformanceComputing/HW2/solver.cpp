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


int kill_signal = 0;
/*************************** solver.h functions ************************/


void seq_solver(unsigned int n, std::vector<std::vector<unsigned int> >& all_solns) {

	std::vector<int> curr(n, -1);
	search_full(all_solns, curr, n, 0);
}

void nqueen_master(	unsigned int n,
					unsigned int k,
					std::vector<std::vector<unsigned int> >& all_solns) {


	int num_procs;
	int num_sol;
	int flag;
	MPI_Status stat;
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	bool is_found = true;

	/******************* STEP 1: Send one partial solution to each worker ********************/
	// Initialize the first partial solution
	std::vector<int> curr = search_step(n, k);

	for (int i = 1; i < num_procs; ++i)
	{
		// create a partial solution
		if (i > 1)
			is_found = search_partial(n, k, curr, int(k-1));
		
		if (!is_found) break;

		

		// for (unsigned int i = 0; i < k; ++i)
		// {
		// 	std::cout<<curr[i];
		// }
		// std::cout<<std::endl;

		// send partial solution to a worker (vector<int>)
		std::vector<int> partial_sol = curr;
		MPI_Send( &partial_sol[0], k, MPI_INT, i, 111, MPI_COMM_WORLD);
	}


	/******************* STEP 2: Send partial solutions to workers as they respond ********************/
	while(true)
	{
		// receive completed work from a worker processor (unsigned int[][])
		MPI_Iprobe(MPI_ANY_SOURCE, 111, MPI_COMM_WORLD, &flag, &stat);
		MPI_Get_count(&stat, MPI_UNSIGNED, &num_sol);
		
		if (!flag)
		{
			if (!is_found) break; 
			else continue;
		}
		
		std::cout<<num_sol<<std::endl;
		unsigned int** sol_list = new unsigned int*[num_sol/n];
		for(unsigned int i = 0; i < num_sol/n; ++i) 
			sol_list[i] = new unsigned int[n];

		MPI_Recv( &(sol_list[0][0]), num_sol, MPI_UNSIGNED, stat.MPI_SOURCE, stat.MPI_TAG, MPI_COMM_WORLD, &stat);

		// store the received sol_list (unsigned int[][]) in all_solns (vector<vector<unsigned int>>)
		
		for (unsigned int i = 0; i < num_sol/n; ++i)
		{
			for (unsigned int j = 0; j < n; ++j)
			{ 
				std::cout<<sol_list[i][j];
			}
			std::cout<<std::endl;
		}
		std::cout<<"====="<<std::endl;

		for (unsigned int i = 0; i < num_sol/n; ++i){
			std::vector<unsigned int> v;
			for (unsigned int j = 0; j < n; ++j)
				v.push_back(sol_list[i][j]);
			all_solns.push_back(v);
		}

		for (unsigned int i = 0; i < num_sol/n; i++)
			delete[] sol_list[i];
		delete[] sol_list;


		// create a partial solution 
		if (is_found)
			is_found = search_partial(n, k, curr, int(k-1));
		
		// send partial solution to the word that responded
		if (is_found){
			std::vector<int> partial_sol = curr;
			MPI_Send( &partial_sol[0], k, MPI_INT, stat.MPI_SOURCE, stat.MPI_TAG, MPI_COMM_WORLD);
		}
	}

	/********************** STEP 3: Terminate **************************/
	
	// Send termination signals to each worker processor
	for (int i = 0; i < all_solns.size(); ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			std::cout<<all_solns[i][j];
		}
		std::cout<<std::endl;
	}
	std::cout<<"====="<<std::endl;


	for (int i = 1; i < num_procs; ++i)
	{
		std::vector<int> partial_sol(k, -1);
		MPI_Send( &partial_sol[0], k, MPI_INT, i, 111, MPI_COMM_WORLD);
	}
}


void nqueen_worker(	unsigned int n,
					unsigned int k) {



	// TODO: Implement this function

	// Following is a general high level layout that you can follow (you are not obligated to design your solution in this manner. This is provided just for your ease).

	MPI_Status stat;
	std::vector<int> sol(k);
	std::cout<<kill_signal<<std::endl;
	int flag;
	
	while(true) {


		MPI_Recv(&sol[0], k, MPI_INT, 0, 111, MPI_COMM_WORLD, &stat);
		if(sol[0] == -1) break;
		sol.resize(n, -1);
		
		// search full solution
		std::vector<std::vector<unsigned int> > sols;
		search_full(sols, sol, n, k);

		// convert 2D unsigned int vector to 2D unsigned int array
		unsigned int** sol_list = new unsigned int*[sols.size()];
		for(unsigned int i = 0; i < sols.size(); ++i) {
			sol_list[i] = new unsigned int[n];
			for (unsigned int j = 0; j < n; ++j)
				sol_list[i][j] = sols[i][j];
		}

		// std::cout<<sols.size()*n<<std::endl;
		// for(unsigned int i = 0; i < sols.size(); ++i) {
		// 	for (unsigned int j = 0; j < n; ++j)
		// 		std::cout<<sol_list[i][j];
		// 	std::cout<<std::endl;
		// }


		// send 2D unsigned int array
		MPI_Send(&(sol_list[0][0]), sols.size()*n, MPI_UNSIGNED, 0, stat.MPI_TAG, MPI_COMM_WORLD);
		for (unsigned int i = 0; i <sols.size(); i++)
			delete[] sol_list[i];
		delete[] sol_list;
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

		if (curr_row + curr_col - row < int(k)){
			if (curr[curr_row + curr_col - row] == row) 
				return false;}

		if (-curr_row + curr_col + row >= 0 && -curr_row + curr_col + row < int(k)){
			if (curr[-curr_row + curr_col + row] == row) 
				return false;}
	}
		

	return true;
}

// Start from root node to right most node at level k (the initialization of partial solution)
std::vector<int> search_step(unsigned int n, unsigned int k){
	
	std::vector<int> curr(k, -1);
	
	for (unsigned int col = 0; col < k; ++col)
	{
		for (unsigned int row = 0; row < n; ++row)
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

	for (unsigned int row = curr[col]+1; row < n; ++row)
	{
		if (!is_valid(curr, row, col, k)) 
			continue;

		curr[col] = row;
		if (col == int(k-1)) 
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
		std::vector<unsigned int> new_sol(curr.begin(), curr.end());
		sols.push_back(new_sol);
		return;
	}

	for (unsigned int row = 0; row < n; ++row)
	{
		if (!is_valid(curr, row, col, n)) 
			continue;

		curr[col] = row;
		search_full(sols, curr, n, col+1);
		curr[col] = -1;
	}
}





