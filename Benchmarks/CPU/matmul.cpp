#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Generate random matrices with variable sizes
std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> generateRandomMatrices(std::pair<int, int> sizeRange) {
    int minSize = sizeRange.first;
    int maxSize = sizeRange.second;
    int rowsA = std::rand() % (maxSize - minSize + 1) + minSize;
    int colsA = std::rand() % (maxSize - minSize + 1) + minSize;
    int colsB = std::rand() % (maxSize - minSize + 1) + minSize;

    std::vector<std::vector<double>> matrixA(rowsA, std::vector<double>(colsA));
    std::vector<std::vector<double>> matrixB(colsA, std::vector<double>(colsB));

    // Fill matrices with random values
    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsA; ++j) {
            matrixA[i][j] = static_cast<double>(rand()) / RAND_MAX; // Random value between 0 and 1
        }
    }

    for (int i = 0; i < colsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            matrixB[i][j] = static_cast<double>(rand()) / RAND_MAX; // Random value between 0 and 1
        }
    }

    return std::make_pair(matrixA, matrixB);
}

// Matrix multiplication
std::vector<std::vector<double>> matrixMultiply(const std::vector<std::vector<double>>& matrixA, const std::vector<std::vector<double>>& matrixB) {
    int rowsA = matrixA.size();
    int colsA = matrixA[0].size();
    int colsB = matrixB[0].size();

    std::vector<std::vector<double>> result(rowsA, std::vector<double>(colsB, 0.0));

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            for (int k = 0; k < colsA; ++k) {
                result[i][j] += matrixA[i][k] * matrixB[k][j];
            }
        }
    }

    return result;
}

int main() {
    std::srand(std::time(nullptr)); // Seed the random number generator

    // Example usage with variable input
    auto matrices = generateRandomMatrices(std::make_pair(10, 1000));
    std::vector<std::vector<double>> result = matrixMultiply(matrices.first, matrices.second);

    // Output the result matrix
    

    return 0;
}
