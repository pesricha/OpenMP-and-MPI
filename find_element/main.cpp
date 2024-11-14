#include<iostream>
#include<vector>
#include<chrono>

typedef std::vector<int> vec;

const int ARRAY_SIZE = 1000000;
const int MIN_ELEM_SIZE = 1;
const int MAX_ELEM_SIZE = 5000000;
const int NUM_RUNS = 20;

void randomizeArray(vec& A, int N)
{
    for (int i = 0; i < N; i++) 
        A[i] = std::rand() % (MAX_ELEM_SIZE - MIN_ELEM_SIZE + 1) + MIN_ELEM_SIZE;
}

int main(int argc, char** argv)
{
    std::srand(static_cast<unsigned>(std::time(0)));
    vec array(ARRAY_SIZE);
    
    std::chrono::duration<double, std::milli> total_time_seq = std::chrono::duration<double>(0.0);

    for (int i = 0; i < 20; i++)
    {
        int index = -1;
        randomizeArray(array, ARRAY_SIZE);
        int elem = std::rand() % (MAX_ELEM_SIZE - MIN_ELEM_SIZE + 1) + MIN_ELEM_SIZE;
        auto start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < ARRAY_SIZE; j++)
        {
            if (array[j] == elem)
            {   
                index = j;
                break;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        if (index != -1)
        {
            total_time_seq += end - start;
            std::cout << "array[index] = " << array[index] 
            << " | TARGET = " << elem
            << " | index = " << index
            << std::endl;
        }
        else std::cout << elem << " not found in the array" << std::endl;
        
    }

    double avg_time_seq = total_time_seq.count() / NUM_RUNS;
    std::cout << "====================================================";
    std::cout << " | Avg Time (ms) = " << avg_time_seq << std::endl;
    
}
