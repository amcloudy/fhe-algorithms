#include "../../src/poly_eval.h"
#include "../../src/utils/config_loader.h"
#include <chrono>

bool run_polyeval_benchmark_func(std::ostream& out) {
    ConfigLoader config("config.yaml");
    for (auto ringDim : config.GetPolyEvalRingDims()) {
        for (auto vecSize : config.GetPolyEvalVectorSizes()) {
            for (const auto& function : config.GetPolyEvalFunctionVariants()) {
                auto start = std::chrono::high_resolution_clock::now();
                bool success = RunPolynomialEvaluation(function, ringDim, vecSize, config.GetPolyEvalMaxVectorValue());
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
