#include <iostream>
#include <fstream>
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

    std::ofstream outfile("text_file.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error creating file." << std::endl;
        return 1;
    }

    for (int i = 0; i < sortedData.size(); ++i) {
        outfile << sortedData[i] << std::endl;
    }
    outfile.close();

    // Delete the file
    if (std::remove("text_file.txt") != 0) {
        std::cerr << "Error deleting file." << std::endl;
        return 1;
    }

    return 0;
}
