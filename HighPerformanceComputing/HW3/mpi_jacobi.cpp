/**
 * @file    mpi_jacobi.cpp
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements MPI functions for distributing vectors and matrixes,
 *          parallel distributed matrix-vector multiplication and Jacobi's
 *          method.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */

#include "mpi_jacobi.h"
#include "jacobi.h"
#include "utils.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <vector>

/*
 * TODO: Implement your solutions here
 */


void distribute_vector(const int n, double* input_vector, double** local_vector, MPI_Comm comm)
{
    // get current rank
    int w_rank;
    MPI_Comm_rank(comm, &w_rank);

    // get process coords in cartesian topology
    int coords[2];
    MPI_Cart_coords(comm, w_rank, 2, coords);

    // creates new communicators based on colors and keys, processors with the same colors are in the same communicator 
    MPI_Comm col_comm;
    MPI_Comm_split(comm, coords[1], coords[0], &col_comm);
    MPI_Barrier(comm);

    
    if(coords[1] == 0){

        // compute sendcounts and displs
        // sendcounts: integer array (of length group size) specifying the number of elements to send to each processor
        // displs: integer array (of length group size). Entry i specifies the displacement (relative to sendbuf from which to take the outgoing data to process i
        int p, curr_rank;
        MPI_Comm_size(col_comm, &p);
        MPI_Comm_rank(col_comm, &curr_rank);

        int* sendcounts, displs;
        sendcounts = (int*) malloc(p*(sizeof(int)));
        displs = (int*) malloc(p*(sizeof(int)));
        displs[0] = 0;

        for (int rank = 0; rank < p; ++rank)
        {
            sendcounts[rank] = block_decompose(n, p, rank);
            if(rank > 0)
                displs[rank] = displs[rank-1] + sendcounts[rank-1];
        }

        // compute recvbuf and recvcount
        *local_vector = (double *) malloc(sendcounts[curr_rank]*sizeof(double));

        // Scatter from processor (0, 0) 
        MPI_Scatterv(input_vector, sendcounts, displs,
                 MPI_DOUBLE, *local_vector, sendcounts[curr_rank],
                 MPI_DOUBLE, 0, col_comm);

        free(sendcounts);
        free(displs);
    }

    MPI_Comm_free(&col_comm);
    MPI_Barrier(comm);

}


// gather the local vector distributed among (i,0) to the processor (0,0)
void gather_vector(const int n, double* local_vector, double* output_vector, MPI_Comm comm)
{

    // get current rank
    int w_rank;
    MPI_Comm_rank(comm, &w_rank);

    // get process coords in cartesian topology
    int coords[2];
    MPI_Cart_coords(comm, w_rank, 2, coords);

    // creates new communicators based on colors and keys, processors with the same colors are in the same communicator 
    MPI_Comm col_comm;
    MPI_Comm_split(comm, coords[1], coords[0], &col_comm);
    MPI_Barrier(comm);

    if(coords[1] == 0){

        // compute sendcounts and displs
        // sendcounts: integer array (of length group size) specifying the number of elements to send to each processor
        // displs: integer array (of length group size). Entry i specifies the displacement (relative to sendbuf from which to take the outgoing data to process i
        int p, curr_rank;
        MPI_Comm_size(col_comm, &p);
        MPI_Comm_rank(col_comm, &curr_rank);

        int* recvcounts, displs;
        recvcounts = (int*) malloc(p*(sizeof(int)));
        displs = (int*) malloc(p*(sizeof(int)));
        displs[0] = 0;

        for (int rank = 0; rank < p; ++rank)
        {
            recvcounts[rank] = block_decompose(n, p, rank);
            if(rank > 0)
                displs[rank] = displs[rank-1] + recvcounts[rank-1];
        }

        // Scatter from processor (0, 0) 
        MPI_Gatherv(local_vector, recvcounts[curr_rank], MPI_DOUBLE,
                output_vector, recvcounts, displs,
                MPI_DOUBLE, 0, col_comm);

        free(recvcounts);
        free(displs);
    }

    MPI_Comm_free(&col_comm);
    MPI_Barrier(comm);


}

void distribute_matrix(const int n, double* input_matrix, double** local_matrix, MPI_Comm comm)
{

    // get current rank
    int w_rank;
    MPI_Comm_rank(comm, &w_rank);

    // get process coords in cartesian topology
    int coords[2];
    MPI_Cart_coords(comm, w_rank, 2, coords);

    // creates new communicators based on colors and keys, processors with the same colors are in the same communicator 
    MPI_Comm col_comm, row_comm;
    MPI_Comm_split(comm, coords[1], coords[0], &col_comm);
    MPI_Comm_split(comm, coords[0], coords[1], &row_comm);
    int blocksize_col = block_decompose(n, col_comm);


    // ======= distribute from data from processor (0, 0) to all processors in the first column ========
    if(coords[1] == 0){

        // compute block size in col dimension for current rank
        int p, curr_rank;
        MPI_Comm_size(col_comm, &p);
        MPI_Comm_rank(col_comm, &curr_rank);

        // compute sendcounts and displs
        int* sendcounts, displs;
        sendcounts = (int*) malloc(p*(sizeof(int)));
        displs = (int*) malloc(p*(sizeof(int)));
        displs[0] = 0;

        for (int rank = 0; rank < p; ++rank)
        {
            sendcounts[rank] = n * block_decompose(n, p, rank);
            if(rank > 0)
                displs[rank] = displs[rank-1] + sendcounts[rank-1];
        }

        // compute recvbuf and recvcount
        double *first_col =(double *) malloc(sendcounts[curr_rank]*sizeof(double));

        // Scatter from processor (0, 0) 
        MPI_Scatterv(input_matrix, sendcounts, displs,
                 MPI_DOUBLE, *first_col, sendcounts[curr_rank],
                 MPI_DOUBLE, 0, col_comm);

        free(sendcounts);
        free(displs);
    }

    MPI_Comm_free(&col_comm);
    MPI_Barrier(comm);

    
    // ========= distribute from data from the first column along each row dimension ========
    
    // compute block size in col dimension for current rank
    int p, curr_rank;
    MPI_Comm_size(row_comm, &p);
    MPI_Comm_rank(row_comm, &curr_rank);
    
    // compute sendcounts and displs
    int* sendcounts, displs;
    sendcounts = (int*) malloc(p*(sizeof(int)));
    displs = (int*) malloc(p*(sizeof(int)));
    displs[0] = 0;

    for (int rank = 0; rank < p; ++rank)
    {
        sendcounts[rank] = block_decompose(n, p, rank);
        if(rank > 0)
            displs[rank] = displs[rank-1] + sendcounts[rank-1];
    }

    // compute recvbuf and recvcount
    *local_matrix = (double*) malloc(blocksize_col*sendcounts[curr_rank]*sizeof(double));

    // Scatter from processor (0, 0) 
    for (int i = 0; i < blocksize_col; ++i)
    {
        MPI_Scatterv(&first_col[i * n], sendcounts, displs,
             MPI_DOUBLE, &(*local_matrix)[i * sendcounts[curr_rank]], sendcounts[curr_rank],
             MPI_DOUBLE, 0, row_comm);
    }
    
    free(sendcounts);
    free(displs);
    MPI_Comm_free(&row_comm);
    MPI_Barrier(comm);
}


void transpose_bcast_vector(const int n, double* col_vector, double* row_vector, MPI_Comm comm)
{
    // get current rank
    int w_rank;
    MPI_Comm_rank(comm, &w_rank);
    

    // get process coords in cartesian topology
    int coords[2];
    MPI_Cart_coords(comm, w_rank, 2, coords);

    // creates new communicators based on colors and keys, processors with the same colors are in the same communicator 
    MPI_Comm col_comm, row_comm;
    MPI_Comm_split(comm, coords[1], coords[0], &col_comm);
    MPI_Comm_split(comm, coords[0], coords[1], &row_comm);

    // get source processor at (coords[0], coords[0]) block size
    int p;
    MPI_Comm_size(col_comm, &p);
    int blocksize_col = block_decompose(n, p, coords[0]); 
    int blocksize_row = block_decompose(n, p, coords[1]);

    // processor at (0, 0) no need to send and recv
    if (coords[0] == 0 && coords[1] == 0)
        memcpy(row_vector, col_vector, blocksize_col * sizeof(double));
    // send data from processor at (coords[0], coords[1]) to processor at (coords[0], coords[0])
    else if(coords[1] == 0)
        MPI_Send(col_vector, blocksize_col, MPI_DOUBLE, coords[0], 0, row_comm);
    // receive data from processor at (coords[0], coords[1]) to processor at (coords[0], coords[0])
    else if(coords[0] == coords[1])
        MPI_Recv(row_vector, blocksize_col, MPI_DOUBLE, 0, 0, row_comm, MPI_STATUS_IGNORE);

    MPI_Barrier(comm);


    // broadcast data in processor at diag (stored in row vector) along its col
    MPI_Bcast(row_vector, blocksize_row, MPI_DOUBLE, coords[1], col_comm);
    MPI_Comm_free(&col_comm);
    MPI_Comm_free(&row_comm);
    MPI_Barrier(comm);
}


void distributed_matrix_vector_mult(const int n, double* local_A, double* local_x, double* local_y, MPI_Comm comm)
{
    // TODO
}

// Solves Ax = b using the iterative jacobi method
void distributed_jacobi(const int n, double* local_A, double* local_b, double* local_x,
                MPI_Comm comm, int max_iter, double l2_termination)
{
    
    // Compute D and distribute D on the processors along the first column

    // Compute and distribute R

    // Initialize x

    // Start iteratively update x 
    // for (int i = 0; i < max_iter; ++i)
    // {
    //     // exit if termination criteria is met
    //     // make sure all processor know that they should exit by using MPI_Allreduce
    //     if (l2 <= l2_termination)
    //         break;
    // }
}


// wraps the distributed matrix vector multiplication
void mpi_matrix_vector_mult(const int n, double* A,
                            double* x, double* y, MPI_Comm comm)
{
    // distribute the array onto local processors!
    double* local_A = NULL;
    double* local_x = NULL;
    distribute_matrix(n, &A[0], &local_A, comm);
    distribute_vector(n, &x[0], &local_x, comm);

    // allocate local result space
    double* local_y = new double[block_decompose_by_dim(n, comm, 0)];
    distributed_matrix_vector_mult(n, local_A, local_x, local_y, comm);

    // gather results back to rank 0
    gather_vector(n, local_y, y, comm);
}

// wraps the distributed jacobi function
void mpi_jacobi(const int n, double* A, double* b, double* x, MPI_Comm comm,
                int max_iter, double l2_termination)
{
    // distribute the array onto local processors!
    double* local_A = NULL;
    double* local_b = NULL;
    distribute_matrix(n, &A[0], &local_A, comm);
    distribute_vector(n, &b[0], &local_b, comm);

    // allocate local result space
    double* local_x = new double[block_decompose_by_dim(n, comm, 0)];
    distributed_jacobi(n, local_A, local_b, local_x, comm, max_iter, l2_termination);

    // gather results back to rank 0
    gather_vector(n, local_x, x, comm);
}
