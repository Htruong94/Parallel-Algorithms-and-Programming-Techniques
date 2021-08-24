// Workshop 2 - Calculate PI by integrating 1/(1+x^2)
// w2.serial.cpp
// Synchronized Version

#include <iostream>
#include <iomanip>
#include <omp.h>
#define NUM_THREADS 1

// report system time
//
void reportTime(const char* msg, double span) {
    //converting to milliseconds
    std::cout << msg << " - took - " <<
        span*1000 << " milliseconds" << std::endl; 
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << argv[0] << ": invalid number of arguments\n";
        std::cerr << "Usage: " << argv[0] << "  no_of_slices\n";
        return 1;
    }
    int n = std::atoi(argv[1]);

    double ts = omp_get_wtime(); //time measured in seconds
    int nthreads = 0;
    double pi = 0;
    omp_set_num_threads(NUM_THREADS);
    double step = 1.0 / (double)n;
#pragma omp parallel
    {
        int i, id, nthrds;
        double x, sum;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0) {
            nthreads = nthrds;
        }
        for (i = id, sum = 0.0; i < n; i = i + nthrds) {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp critical
            pi += sum * step;
    }

    double te = omp_get_wtime();

    std::cout << "n = " << n <<
        std::fixed << std::setprecision(15) <<
        "\n pi(exact) = " << 3.141592653589793 <<
        "\n pi(calcd) = " << pi << std::endl;
    reportTime("Integration", te - ts);
}