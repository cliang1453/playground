/**
 * @file    jacobi.cpp
 * @author  Patrick Flick <patrick.flick@gmail.com>
 * @brief   Implements matrix vector multiplication and Jacobi's method.
 *
 * Copyright (c) 2014 Georgia Institute of Technology. All Rights Reserved.
 */
#include "jacobi.h"

/*
 * TODO: Implement your solutions here
 */

// my implementation:
#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;

// Calculates y = A*x for a square n-by-n matrix A, and n-dimensional vectors x
// and y
void matrix_vector_mult(const int n, const double* A, const double* x, double* y)
{
    for (int r = 0; r < n; ++r)
    {
    	y[r] = 0;
    	for (int c = 0; c < n; ++c)
    		y[r] += A[r * n + c] * x[c];
    }
}

// Calculates y = A*x for a n-by-m matrix A, a m-dimensional vector x
// and a n-dimensional vector y
void matrix_vector_mult(const int n, const int m, const double* A, const double* x, double* y)
{
    for (int r = 0; r < n; ++r)
    {
    	y[r] = 0;
    	for (int c = 0; c < m; ++c)
    		y[r] += A[r * n + c] * x[c];
    }
}

// implements the sequential jacobi method
void jacobi(const int n, double* A, double* b, double* x, int max_iter, double l2_termination)
{

	// Compute D and R
	double D[n];
	double R[n*n];
    memcpy(R, A, sizeof(double)*(n*n));
	
	for (int r = 0; r < n; ++r)
	{
        D[r] = A[r * n + r];
        R[r * n + r] = 0;
        x[r] = 0;
	}

	// Update x iteratively
	double Rx[n];
	double Ax[n];

    for (int i = 0; i < max_iter; ++i)
    {
    	matrix_vector_mult(n, R, x, Rx);

    	for (int r = 0; r < n; ++r)
    		x[r] = (b[r] - Rx[r])/D[r];
    	
    	matrix_vector_mult(n, A, x, Ax);

    	double l2 = 0;
    	for (int r = 0; r < n; ++r)
    		l2 += pow(b[r] - Ax[r], 2);
    	l2 = pow(l2, 0.5);

    	if (l2 <= l2_termination)
    		break;
    }
}
