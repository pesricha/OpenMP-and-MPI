#include<iostream>
#include<vector>
#include<chrono>
#include<random>
#include<iomanip>

typedef std::vector<int> vec;

const int N_VALUES[] = {10000, 20000, 30000};
const int THREAD_VALUE[] = {2, 4, 8, 16, 32, 64};

void printVector1D(vec& A)
{
    for (auto elem: A) std::cout << elem << " ";
    std::cout << std::endl;
}

void prefixSum(vec& A, vec& B)
{
    int N = A.size();
    B[0] = A[0];
    for (int i=1 ; i<A.size(); i++) B[i] = B[i-1] + A[i];
}

int main()
{
    for (const auto N: N_VALUES){
        
        std::chrono::duration<double> total_time = std::chrono::duration<double>(0.0);
        vec A(N, 1);
        vec B(N);

        for (int i = 0; i < 5; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            prefixSum(A, B);
            auto stop = std::chrono::high_resolution_clock::now();
            
            std::chrono::duration<double> time = stop - start;
            total_time += time;
        }

        total_time = total_time / 5;

        std::cout << std::scientific;
        std::cout << "Value: " << B[N-1] <<" | Average time: " << total_time.count() <<" seconds" << std::endl;
    }
        
    return 0;
}
