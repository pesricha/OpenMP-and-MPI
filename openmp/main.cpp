#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <omp.h>

typedef std::vector<int> vec;

const int N_VALUES[] = {10000, 20000, 30000};
const int THREAD_VALUE[] = {2, 4, 8, 16, 32, 64};

void prefixSumOMP(vec& A, vec& B, int n, int thread_count)
{
    vec offsets(thread_count, 0);

    #pragma omp parallel num_threads(thread_count)
    {
        int threadId = omp_get_thread_num();
        int start = threadId * (n / thread_count);
        int end = (threadId + 1) * (n / thread_count);

        B[start] = A[start];

        for (int i = start + 1; i < end; i++)
        {
            B[i] = A[i] + B[i - 1];
        }
        
        #pragma omp barrier

        offsets[threadId] = (threadId > 0) ? B[start - 1] : 0;

        #pragma omp barrier

        #pragma omp single
        {
            for (int i = 1; i < thread_count; i++)
            {
                offsets[i] = offsets[i] + offsets[i - 1];
            }
        }        

        #pragma omp barrier

        #pragma omp simd
        for (int i = start; i < end; i++)
        {
            B[i] = B[i] + offsets[threadId];
        }

        #pragma omp barrier
    }
}

void prefixSum(vec& A, vec& B)
{
    int N = A.size();
    B[0] = A[0];
    for (int i = 1; i < N; i++) B[i] = B[i - 1] + A[i];
}

int main(int argc, char** argv)
{   
    std::cout << "=============================================================================" << std::endl;

    for (auto thread_count : THREAD_VALUE)
    {
        for (const auto N : N_VALUES)
        {
            std::chrono::duration<double, std::milli> total_time_omp = std::chrono::duration<double>(0.0);
            std::chrono::duration<double, std::milli> total_time_seq = std::chrono::duration<double>(0.0);
            vec A(N);
            vec B(N, 0);
            vec B_seq(N, 0);

            // Initialize A to avoid overflow.
            for (int i = 0; i < N; i++) A[i] = std::rand() % (INT32_MAX / N);

            // Measure sequential time
            for (int i = 0; i < 5; i++)
            {
                auto start_seq = std::chrono::high_resolution_clock::now();
                prefixSum(A, B_seq);
                auto stop_seq = std::chrono::high_resolution_clock::now();

                total_time_seq += (stop_seq - start_seq);
            }
            total_time_seq /= 5;

            // Measure parallel time
            for (int i = 0; i < 5; i++)
            {
                auto start_omp = std::chrono::high_resolution_clock::now();
                prefixSumOMP(A, B, N, thread_count);
                auto stop_omp = std::chrono::high_resolution_clock::now();

                total_time_omp += (stop_omp - start_omp);
            }
            total_time_omp /= 5;

            // Calculate speedup
            double speedup = total_time_seq.count() / total_time_omp.count();

            // Display results
            std::cout << "Threads: " << thread_count << " | N: " << N
                      << " | Avg time (Seq): " << total_time_seq.count() << " ms"
                      << " | Avg time (OMP): " << total_time_omp.count() << " ms"
                      << " | Speedup: " << speedup << std::endl;

            // // Print last elements for verification
            // std::cout << "Sequential Result (last element): " << B_seq.back() << std::endl;
            // std::cout << "Parallel Result (last element):   " << B.back() << std::endl;
            // std::cout << "-----------------------------------------------------------------------------" << std::endl;
        }

        std::cout << "=============================================================================" << std::endl;
    }

    return 0;
}
