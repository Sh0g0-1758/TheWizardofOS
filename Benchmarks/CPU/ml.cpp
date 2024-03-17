#include <iostream>
#include <vector>
#include <random>
#include <algorithm> // for std::transform
#include <numeric>   // for std::inner_product

// Function to generate random numbers within a specified range (inclusive)
double random_double(double min, double max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

// Function to calculate the mean of a vector
double mean(const std::vector<double>& vec) {
    double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    return sum / vec.size();
}

// Function to perform matrix multiplication (dot product)
std::vector<double> matrix_multiply(const std::vector<std::vector<double>>& A, const std::vector<double>& B) {
    std::vector<double> result(A.size(), 0.0);
    for (size_t i = 0; i < A.size(); ++i) {
        for (size_t j = 0; j < A[i].size(); ++j) {
            result[i] += A[i][j] * B[j];
        }
    }
    return result;
}

// Function to perform linear regression without Eigen library
std::vector<double> ml_load_cpp() {
    // Set random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define range of samples and features
    const int min_samples = 1000;
    const int max_samples = 10000;
    const int min_features = 10;
    const int max_features = 100;

    // Randomly select number of samples and features
    int num_samples = std::uniform_int_distribution<int>(min_samples, max_samples)(gen);
    int num_features = std::uniform_int_distribution<int>(min_features, max_features)(gen);

    // Generate random data
    std::vector<std::vector<double>> X(num_samples, std::vector<double>(num_features));
    std::vector<double> y(num_samples);
    for (int i = 0; i < num_samples; ++i) {
        for (int j = 0; j < num_features; ++j) {
            X[i][j] = random_double(0.0, 1.0);
        }
        y[i] = random_double(0.0, 1.0);
    }

    // Calculate mean of X
    std::vector<double> mean_x(num_features);
    for (int j = 0; j < num_features; ++j) {
        std::vector<double> column_values;
        for (int i = 0; i < num_samples; ++i) {
            column_values.push_back(X[i][j]);
        }
        mean_x[j] = mean(column_values);
    }

    // Center X by subtracting the mean
    for (int i = 0; i < num_samples; ++i) {
        for (int j = 0; j < num_features; ++j) {
            X[i][j] -= mean_x[j];
        }
    }

    // Calculate transpose of X
    std::vector<std::vector<double>> X_transpose(num_features, std::vector<double>(num_samples));
    for (int i = 0; i < num_samples; ++i) {
        for (int j = 0; j < num_features; ++j) {
            X_transpose[j][i] = X[i][j];
        }
    }

    // Calculate pseudo-inverse of X_transpose * X
    std::vector<std::vector<double>> X_pinv(num_features, std::vector<double>(num_features));
    for (int i = 0; i < num_features; ++i) {
        for (int j = 0; j < num_features; ++j) {
            for (int k = 0; k < num_samples; ++k) {
                X_pinv[i][j] += X_transpose[i][k] * X[k][j];
            }
        }
    }

    // Perform LU decomposition for inversion (using Gaussian elimination)
    std::vector<std::vector<double>> identity(num_features, std::vector<double>(num_features, 0.0));
    for (int i = 0; i < num_features; ++i) {
        identity[i][i] = 1.0;
    }

    for (int i = 0; i < num_features; ++i) {
        double pivot = X_pinv[i][i];
        for (int j = i + 1; j < num_features; ++j) {
            double factor = X_pinv[j][i] / pivot;
            for (int k = 0; k < num_features; ++k) {
                X_pinv[j][k] -= factor * X_pinv[i][k];
                identity[j][k] -= factor * identity[i][k];
            }
        }
    }

    // Back substitution to get the inverse
    for (int i = num_features - 1; i > 0; --i) {
        for (int j = i - 1; j >= 0; --j) {
            double factor = X_pinv[j][i] / X_pinv[i][i];
            for (int k = 0; k < num_features; ++k) {
                X_pinv[j][k] -= factor * X_pinv[i][k];
                identity[j][k] -= factor * identity[i][k];
            }
        }
    }

    // Normalize the identity matrix to get the pseudo-inverse
    for (int i = 0; i < num_features; ++i) {
        double diagonal = X_pinv[i][i];
        for (int j = 0; j < num_features; ++j) {
            X_pinv[i][j] /= diagonal;
        }
        for (int j = 0; j < num_features; ++j) {
            identity[i][j] /= diagonal;
        }
    }

    // Calculate weights (beta) using matrix multiplication
    std::vector<double> beta(num_features);
    for (int i = 0; i < num_features; ++i) {
        for (int j = 0; j < num_features; ++j) {
            beta[i] += X_pinv[i][j] * X_transpose[j][0]; // Assuming y is a column vector
        }
    }

    // Generate random test data
    int num_test_samples = std::uniform_int_distribution<int>(1, 10)(gen);
    std::vector<std::vector<double>> X_test(num_test_samples, std::vector<double>(num_features));
    for (int i = 0; i < num_test_samples; ++i) {
        for (int j = 0; j < num_features; ++j) {
            X_test[i][j] = random_double(0.0, 1.0);
        }
    }

    // Center test data using previously calculated mean
    for (int i = 0; i < num_test_samples; ++i) {
        for (int j = 0; j < num_features; ++j) {
            X_test[i][j] -= mean_x[j];
        }
    }

    // Make predictions using trained weights (beta) and test data
    std::vector<double> predictions(num_test_samples);
    for (int i = 0; i < num_test_samples; ++i) {
        for (int j = 0; j < num_features; ++j) {
            predictions[i] += beta[j] * X_test[i][j];
        }
    }

    return predictions;
}

int main() {
    std::vector<double> predictions = ml_load_cpp();

    return 0;
}
