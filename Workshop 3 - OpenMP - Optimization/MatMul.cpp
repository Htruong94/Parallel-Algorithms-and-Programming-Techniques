// Workshop 3 - Matrix Multiplication - Optimization
// MatMul.cpp
// 2020.09.30
// Chris Szalwinski

#include "MatMul.h"

// matmul returns the product c = a * b
//
void matmul(const row a[], const row b[], row c[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

// checksum returns the sum of the coefficients in matrix x[rows][cols]
//
double checksum(const row x[]) {

    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sum += x[i][j];
        }
    }
    return sum;
}

// initialize initializes matrix a[rows][cols]
//
void initialize(row a[]) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            a[i][j] = static_cast<double>(i * j) / (N * N);
        }
    }
}