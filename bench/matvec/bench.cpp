#include "../../src/matvec.h"
#include "../../src/utils/config_loader.h"
#include <chrono>

bool run_matvec_benchmark(const ConfigLoader::MatVecParams& cfg, std::ostream& out) {
    for (auto ringDim : cfg.ringDims) {
        for (auto size : cfg.matrixSizes) {
//            for (const auto& function : cfg.functionVariants) {
            for (const auto& function : {"helib"}) {
                auto start = std::chrono::high_resolution_clock::now();
                bool success = RunMatrixVectorMultiplication(function, ringDim, size, cfg.max_matrix_value, cfg.max_vector_value);
                if (!success) {
                    std::cerr << "❌ Evaluation failed for ringDim=" << ringDim
                              << ", vectorSize=" << size
                              << ", functionType=" << function << "\n";
                    continue;
                }
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();
                out << ringDim << "," << size << "," << ms << "\n";
              }
        }
    }
    return true;
}
