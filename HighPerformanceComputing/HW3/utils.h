/**
 * @file    utils.h
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements common utility/helper functions.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */

/*********************************************************************
 *             You can add new functions to this header.             *
 *********************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <mpi.h>

/*********************************************************************
 * DO NOT CHANGE THE FUNCTION SIGNATURE OF THE FOLLOWING 3 FUNCTIONS *
 *********************************************************************/

inline int block_decompose(const int n, const int p, const int rank)
{
    return n / p + ((rank < n % p) ? 1 : 0);
}

inline int block_decompose(const int n, MPI_Comm comm)
{
    int rank, p;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &p);
    return block_decompose(n, p, rank);
}

inline int block_decompose_by_dim(const int n, MPI_Comm comm, int dim)
{
    // get dimensions
    int dims[2];
    int periods[2];
    int coords[2];
    MPI_Cart_get(comm, 2, dims, periods, coords);
    return block_decompose(n, dims[dim], coords[dim]);
}


/*********************************************************************
 *                  DECLARE YOUR OWN FUNCTIONS HERE                  *
 *********************************************************************/

/**
 * @brief   Perform D = diag(A)
 *
 * @param n     The size of the first dimension.
 * @param m     The size of the second dimension.
 * @param A     A n-by-m matrix represented in row-major order. always assume n <= m. 
 * @param D     The output vector of length n.
 */
inline void matrix_diag(const int n, const int m, const double* A, double* D)
{
    for (int r = 0; r < n; ++r)
        D[r] = A[r * m + r];
}

/**
 * @brief   Perform R = A - diag(A)
 *
 * @param n     The size of the first dimension.
 * @param m     The size of the second dimension.
 * @param A     A n-by-m matrix represented in row-major order. always assume n <= m. 
 * @param R     The output matrix with zero-diagnal of size n-by-m
 */
inline void matrix_zero_diag(const int n, const int m, const double* A, double* R)
{
    memcpy(R, A, n * m * sizeof(double));
    for (int r = 0; r < n; ++r)
        R[r * m + r] = 0;
}

/**
 * @brief   Perform D[i] = 1.0/D[i]
 *
 * @param n     The length of D.
 * @param D     An array of length n.
 * @param D_inv The output array of length n.
 */
inline void elementwise_inv(const int n, const double* D, double* D_inv)
{
    for (int i = 0; i < n; ++i)
        D_inv[i] = 1.0/D[i];
}

#endif // UTILS_H
