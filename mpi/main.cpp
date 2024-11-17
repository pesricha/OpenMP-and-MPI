#include <iostream>
#include <vector>
#include <numeric>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include "mpi.h"

#define ARRAY_SIZE 1000000
#define MAX_ELEM 5000000
#define MIN_ELEM 1
#define NUM_TRIALS 20

typedef std::vector<int> vectorint;
typedef std::vector<double> vectordouble;


int main(int argc, char** argv) {
    MPI_Comm comm;
    MPI_Status status;
    MPI_Request request;

    int rank, size;
    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int local_size = ARRAY_SIZE / size;
    vectorint local_array(local_size);
    vectordouble times(NUM_TRIALS, 0.0); 
    vectorint gloabal_array(ARRAY_SIZE);

    if (rank == 0)
    {
        srand(time(nullptr));
    }

    for (int run = 0; run < NUM_TRIALS; run++)
    {
        if(rank == 0)
        {
            for (int i = 0; i < ARRAY_SIZE; i++)
                gloabal_array[i] = rand() % (MAX_ELEM - MIN_ELEM + 1) + MIN_ELEM;
        }
    
        MPI_Scatter(gloabal_array.data(), local_size, MPI_INT, local_array.data(), local_size, MPI_INT, 0, comm);

        int target = 0;
        if (rank == 0) target = rand() % (MAX_ELEM - MIN_ELEM + 1) + MIN_ELEM;
        MPI_Bcast(&target, 1, MPI_INT, 0, comm);

        double start_time = MPI_Wtime();

        int local_index = -1;
        for (int i = 0 ; i < local_size; i++)
        {
            if (target == local_array[i])
            {
                local_index = i;
                break;
            }
        }

        int global_index = (local_index != -1) ? rank * local_size + local_index : ARRAY_SIZE + 5;

        int result;
        MPI_Allreduce(&global_index, &result, 1, MPI_INT, MPI_MIN, comm);
        
        double end_time = MPI_Wtime();
        times[run] = end_time - start_time;

        // // or Debugging
        // if (rank == 0) {
        //     std::cout << "Run " << run + 1 << ": Searching " << ARRAY_SIZE 
        //             << " elements (" << local_size << " per process) for target " << target << std::endl;
        //     if (result < ARRAY_SIZE) {
        //         std::cout << "  Element found at global index: " << result 
        //                 << " Array[i]: " << gloabal_array[result] << std::endl;
        //     } else {
        //         std::cout << "  Element not found" << std::endl;
        //     }
        // }
    }

    // Calculate average execution time
    double avg_time = 0.0;
    for (double t : times) {
        avg_time += t;
    }
    avg_time /= NUM_TRIALS;

    // Process 0 gathers and prints results
    if (rank == 0) {
    
    std::cout << "Average time over " << NUM_TRIALS << " runs: " 
              << avg_time*1000 << " ms with " << size << " processes." 
              << std::endl;
    }

    MPI_Finalize();
    return 0;
}