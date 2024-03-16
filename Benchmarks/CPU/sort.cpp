#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

std::vector<int> sortingLoad(int maxSize = 10000) {
    // Initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Generate random size
    std::uniform_int_distribution<> sizeDist(1, maxSize);
    int size = sizeDist(gen);
    
    // Generate random data
    std::uniform_int_distribution<> dataDist(1, 10000);
    std::vector<int> data(size);
    for (int i = 0; i < size; ++i) {
        data[i] = dataDist(gen);
    }
    
    // Sort the data
    std::sort(data.begin(), data.end());
    
    return data;
}

int main() {
    std::vector<int> sortedData = sortingLoad();
    
   
    
    return 0;
}
