#include "../../src/poly_eval.h"
#include "../../src/utils/config_loader.h"
#include <chrono>

bool run_polyeval_benchmark_func(const ConfigLoader::PolyEvalParams& cfg, std::ostream& out) {
    for (auto ringDim : cfg.ringDims) {
        for (auto vecSize : cfg.vectorSizes) {
            for (const auto& function : cfg.functionVariants) {
                auto start = std::chrono::high_resolution_clock::now();
                bool success = RunPolynomialEvaluation(function, ringDim, vecSize, cfg.max_vector_value);
                if (!success) {
                    std::cerr << "❌ Evaluation failed for ringDim=" << ringDim
                              << ", vectorSize=" << vecSize
                              << ", functionType=" << function << "\n";
                    continue;
                }
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();

                out << ringDim << "," << vecSize << "," << function << "," << ms << "\n";
            }
        }
    }
    return true;
}
