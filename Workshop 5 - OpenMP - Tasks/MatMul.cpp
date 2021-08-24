// Workshop 5 - Matrix Multiplication - Tasks - Baseline
 // MatMul.cpp
 // 2018.10.27
 // Chris Szalwinski
 // 2021.07.10
 // Hung Truong - Serial untiled no loop interchange

#include "MatMul.h"
/* Untiled Version
*/
// matmul returns the product c = a * b
//

void matmul(const row a[], const row b[], row c[], int n) {
    {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
    }
}

// End Untiled Version

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
            a[i][j] = 0.0;
        }
        a[i][i] = 1.0;
    }
}