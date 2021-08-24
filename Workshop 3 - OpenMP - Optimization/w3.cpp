// Workshop 3 - Matrix Multiplication - Optimization
 // w3.cpp
 // 2020.09.30
 // Chris Szalwinski

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include "MatMul.h"

void reportTime(const char* msg, std::chrono::steady_clock::duration span);

int main(int argc, char** argv) {
    if (argc != 1) {
        std::cerr << "*** Incorrect number of arguments ***\n";
        std::cerr << "Usage: " << argv[0]
            << "\n";
        return 1;
    }
    std::cout << std::fixed << std::setprecision(2);

    // allocate memory for matrices
    std::chrono::steady_clock::time_point ts, te;

    // matrix ceofficient stored in row major order
    double* a = new double[N * N];
    double* b = new double[N * N];
    double* c = new double[N * N]{ 0 };
    // addresses of the rows in each matrix
    row* aa = reinterpret_cast<row*>(a);
    row* bb = reinterpret_cast<row*>(b);
    row* cc = reinterpret_cast<row*>(c);

    std::cout << "Matrix Multiplication " << N << " by " << N << std::endl;

    // initialize a and b
    ts = std::chrono::steady_clock::now();
    initialize(aa);
    initialize(bb);
    te = std::chrono::steady_clock::now();
    reportTime("initialization", te - ts);

    // matrix multiply cc = aa * bb
    ts = std::chrono::steady_clock::now();
    matmul(aa, bb, cc, N);
    te = std::chrono::steady_clock::now();
    reportTime("execution", te - ts);

    std::cout << "checksum = " << checksum(cc) << std::endl;

    delete[] a;
    delete[] b;
    delete[] c;
}

// reportTime inserts the duration (span) into standard output
//
void reportTime(const char* msg, std::chrono::steady_clock::duration span) {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(span);
    std::cout << msg << ": " <<
        ms.count() << " milliseconds" << std::endl;
}