#include<iostream>
#include<vector>
#include<chrono>
#include<iomanip>
#include<omp.h>

typedef std::vector<int> vec;

const int N_VALUES[] = {10000, 20000, 30000};
const int THREAD_VALUE[] = {2, 4, 8, 16, 32, 64};

void printVector1D(vec& A)
{
    for (auto elem: A) std::cout << elem << " ";
    std::cout << std::endl;
}

void prefixSumOMP(vec& A, vec &B, int n, int thread_count)
{
    vec offsets(thread_count, 0);

    #pragma omp parallel num_threads(thread_count)
    {
        int threadId = omp_get_thread_num();
        int start =  threadId * (n / thread_count);
        int end = (threadId + 1) * (n / thread_count);

        B[start] = A[start];

        for (int i = start + 1; i < end; i++)
        {
            B[i] = A[i] + B[i-1]; 
        }
        
        #pragma omp barrier

        offsets[threadId] = (threadId > 0) ? B[start - 1] : 0;

        #pragma omp barrier

        #pragma omp single
        {
            for (int i = 1; i < thread_count; i++)
            {
                offsets[i] = offsets[i] + offsets[i-1];
            }
        }        

        #pragma omp barrier

        #pragma omp simd
        for(int i = start; i < end ; i++)
        {
            B[i] = B[i] +  offsets[threadId];
        }

        #pragma omp barrier

    }

}

int main(int argc, char** argv)
{   
    std::cout << "=============================================================================" << std::endl;

    for(auto thread_count: THREAD_VALUE)
    {
        for (const auto N: N_VALUES){
            
            std::chrono::duration<double, std::milli> total_time = std::chrono::duration<double>(0.0);
            vec A(N);
            vec B(N, 0);
        
            // Initializing A to avoid overflow.
            for (int i = 0 ; i < N; i++) A[i] = std::rand() % (INT32_MAX/N);

            for (int i = 0; i < 5; i++)
            {
                auto start = std::chrono::high_resolution_clock::now();
                prefixSumOMP(A, B, N, thread_count);
                auto stop = std::chrono::high_resolution_clock::now();
                
                std::chrono::duration<double, std::milli> time = stop - start;
                total_time += time;
            }

            total_time = total_time / 5;

            // std::cout << std::scientific;
            std::cout << "Threads: " << thread_count << " |" << "Value: " << B[N-1] <<" | Average time: " << total_time.count() <<" ms" << std::endl;
        }

        std::cout << "=============================================================================" << std::endl;
    }    

    return 0;
}