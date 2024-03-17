#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#define int long long

void file_io_load() {
    // Generate a large file
    std::ofstream outfile("text_file.txt");

    int size = std::rand() % 9999991 + 10;  
    std::string random_text = "Hello, world! ";
    for (int i = 0; i < size; ++i) {
        outfile << random_text;
    }
    outfile.close();
    std::ofstream appendFile("text_file.txt", std::ios::app);
    appendFile << "Hello, world!";
    appendFile.close();
    std::ifstream infile("text_file.txt");
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();
}

signed main() {
    file_io_load();
    return 0;
}
