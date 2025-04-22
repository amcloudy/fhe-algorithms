#ifndef HELPER_UTILS_H
#define HELPER_UTILS_H

#include <vector>
#include <string>
#include <random>
#include <iomanip>
#include <iostream>


#include "openfhe.h"
#include <memory>
#include <string>
#include <iomanip>

#include "config_loader.h"
#include "scheme_defs.h"

namespace utils {

    inline void print_line(int line_number) {
        std::cout << "Line " << std::setw(3) << line_number << " --> ";
    }
    void PrintHeader(const std::string& title, const std::string& emoji = "🔐");
    void PrintMatrix(const std::vector<std::vector<double>>& matrix);
    void PrintVector(const std::vector<double>& vec);
    void GenerateRandomMatrix(std::vector<std::vector<double>>& matrix, size_t dim, double maxVal);
    void GenerateRandomVector(std::vector<double>& vector, size_t dim, double maxVal);
    void PrintContextSummary(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, FHEScheme scheme);
    void PrintModulusChain(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc);

} // namespace utils

#endif //HELPER_UTILS_H
