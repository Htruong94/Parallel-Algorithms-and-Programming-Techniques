// Workshop 9 - Discretize the Elements of an Array
// Chris Szalwinski
// 2020/11/27
// w9.mpi.c

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "mpi.h"
#define TIME 1

// report processor time
//
void reportTime(const char* msg, int i, double span) {
    printf("Process %d: %-12s took %7.4lf seconds\n", i, msg, span);
}

// discretize data[n] into 0s and 1s
//
void discretize(float* data, int n) {
    for (int i = 0; i < n; i++)
        data[i] = (pow(sin(data[i]), cos(data[i])) +
            pow(cos(data[i]), sin(data[i]))) / 2.0f;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "%s : invalid number of arguments\n"
            "Usage: %s no_of_elements\n", argv[0], argv[0]);
        return 1;
    }

    // retrieve number of elements
    int n = atoi(argv[1]);

    int rank, np;
    //
    // initialize MPI
    //

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int nPerProcess = n / np;
    n = nPerProcess * np;
    int zeroes = 0;
    float* data = NULL;
    float* result = NULL;

    // get MPI wall clock time start

    double begin = MPI_Wtime();

    if (rank == 0) {
        // initialization

        // get MPI allocation time start

        double aBegin = MPI_Wtime();

        data = (float*)malloc(n * sizeof(float));
        result = (float*)malloc(n * sizeof(float));
        // set original data
        for (int i = 0; i < n; i++)
            data[i] = (float)rand() / RAND_MAX;

        // get MPI allocation time end

        double aEnd = MPI_Wtime();

        reportTime("Allocation", 0, aEnd - aBegin);

        // conversion

        // get MPI time start

        double cStart = MPI_Wtime();

        // scatter original data to all workers including master
        MPI_Scatter(data, nPerProcess, MPI_FLOAT,
            MPI_IN_PLACE, 0, MPI_FLOAT, 0, MPI_COMM_WORLD);

        discretize(data, nPerProcess);

        // gather converted data from all workers

        MPI_Gather(data, nPerProcess, MPI_FLOAT,
            result, nPerProcess, MPI_FLOAT, 0, MPI_COMM_WORLD);

        // get MPI time end

        double cEnd = MPI_Wtime();

        // identify 0s and 1s
        for (int i = 0; i < nPerProcess; i++)
            if (data[i] < 0.707f)
                zeroes++;
        for (int i = nPerProcess; i < n; i++)
            if (result[i] < 0.707f)
                zeroes++;

        // report timing statistics

        // error handler declaration

        double totalTime = 0, time = 0.0;
        reportTime("Conversion", 0, cEnd - cStart);
        for (int i = 1; i < np; i++) {

            // collect time for process i
            MPI_Recv(&time, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG,
                MPI_COMM_WORLD, &status);
            reportTime("Conversion", i, time);
            totalTime += time;
        }
        reportTime("Sum of All", 0, totalTime);
    }
    else {
        // worker process
        float* data = (float*)malloc(nPerProcess * sizeof(float));

        // get MPI time
        double st = MPI_Wtime();
        
        // scatter receiver of original data
        MPI_Scatter(NULL, nPerProcess, MPI_INT, data,
            nPerProcess, MPI_FLOAT, 0, MPI_COMM_WORLD);

        discretize(data, nPerProcess);

        // gather converted data and send to master
        MPI_Gather(data, nPerProcess, MPI_FLOAT, NULL,
            nPerProcess, MPI_INT, 0, MPI_COMM_WORLD);

        // get MPI time
        double et = MPI_Wtime();
        double time = et - st;

        free(data);

        // report time
        reportTime("Conversion", 0, time);

        // send time to master
        MPI_Send(&time, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    // get MPI wall clock time end

    double end = MPI_Wtime();

    if (rank == 0) {
        reportTime("Conversion Time - Wall clock", 0, end - begin);

        // get MPI deallocation time start

        double st = MPI_Wtime();

        free(data);
        free(result);

        // get MPI deallocation time end

        double et = MPI_Wtime();

        reportTime("Deallocation", 0, et - st);
        printf("Result: %d = %d (0s) + %d (1s)\n", n, zeroes, n - zeroes);
    }

    // close MPI
    MPI_Finalize();
    return 0;
}