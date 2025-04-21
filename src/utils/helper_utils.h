#ifndef HELPER_UTILS_H
#define HELPER_UTILS_H

#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <iostream>

namespace utils {

    void PrintHeader(const std::string& title, const std::string& emoji = "🔐");
    void PrintMatrix(const std::vector<std::vector<double>>& matrix);
    void PrintVector(const std::vector<double>& vec);
    void GenerateRandomMatrix(std::vector<std::vector<double>>& matrix, size_t dim, double maxVal);
    void GenerateRandomVector(std::vector<double>& vector, size_t dim, double maxVal);

} // namespace utils

#endif //HELPER_UTILS_H
