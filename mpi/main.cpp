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

typedef std::vector<int> vec;

void randomizeArray(vec &A, int N) {
    for (int i = 0; i < N; i++)
        A[i] = std::rand() % (MAX_ELEM - MIN_ELEM + 1) + MIN_ELEM;
}

const int TARGET = 42;

int main(int argc, char** argv) {
    MPI_Comm comm;
    MPI_Status status;
    MPI_Request request;

    int rank, size;
    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    vec array(ARRAY_SIZE);
    std::vector<double> times(NUM_TRIALS); 

    if (rank == 0) {
        std::srand(static_cast<unsigned>(std::time(0)));
        randomizeArray(array, ARRAY_SIZE);
    }

    const int CHUNK_SIZE = ARRAY_SIZE / size;
    vec local_array(CHUNK_SIZE);

    for (int trial = 0; trial < NUM_TRIALS; trial++) {

        // Start timing for the trial
        double start_time = MPI_Wtime();

        // Scatter the array to all processes
        MPI_Scatter(array.data(), CHUNK_SIZE, MPI_INT, local_array.data(), CHUNK_SIZE, MPI_INT, 0, comm);

        int target_found = 0;
        int received_index = -1;
        MPI_Request r;
        MPI_Irecv(&target_found, 1, MPI_INT, MPI_ANY_SOURCE, 69420, comm, &r);

        if (rank == 0) {
            MPI_Request rs;
            MPI_Irecv(&received_index, 1, MPI_INT, MPI_ANY_SOURCE, 420, comm, &rs);
        }

        for (int i = 0; i < CHUNK_SIZE; i++) {
            if (local_array[i] == TARGET || target_found) {
                target_found = 1;
                MPI_Bcast(&target_found, 1, MPI_INT, rank, comm);
                int local_index_adjusted = i + CHUNK_SIZE * rank;
                MPI_Send(&local_index_adjusted, 1, MPI_INT, 0, 420, comm);
                break;
            }
        }

        MPI_Barrier(comm);
        double end_time = MPI_Wtime();
        double elapsed_time = (end_time - start_time) * 1000;  // time in ms

        // Rank 0 collects elapsed time
        if (rank == 0) {
            times[trial] = elapsed_time;
            if (received_index != -1) {
                std::cout << "Trial " << trial+1 << ": Element " << TARGET << " found at Index : " << received_index 
                          << " with A[index] = " << array[received_index] << std::endl;
            } else {
                std::cout << "Trial " << trial+1 << ": Element " << TARGET << " not found" << std::endl;
            }
        }
    }

    // Compute the average time for rank 0
    if (rank == 0) {
        double average_time = std::accumulate(times.begin(), times.end(), 0.0) / NUM_TRIALS;
        std::cout << "Average time taken with " << size << " processes: " << average_time << " ms" << std::endl;
    }

    MPI_Finalize();
    return 0;
}