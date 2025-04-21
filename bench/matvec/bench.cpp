#include "../../src/matvec.h"
#include "../../src/utils/config_loader.h"
#include <chrono>

bool run_matvec_benchmark(const ConfigLoader::MatVecParams& cfg, std::ostream& out) {
    for (auto ringDim : cfg.ringDims) {
        for (auto size : cfg.matrixSizes) {
            auto start = std::chrono::high_resolution_clock::now();
            bool success = RunMatrixVectorMultiplication(ringDim, size, cfg.max_matrix_value, cfg.max_vector_value);
            if (!success) return false;
            auto end = std::chrono::high_resolution_clock::now();
            double ms = std::chrono::duration<double, std::milli>(end - start).count();
            out << ringDim << "," << size << "," << ms << "\n";
        }
    }
    return true;
}
