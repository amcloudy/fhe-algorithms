#include "../../src/matvec.h"
#include "../../src/utils/config_loader.h"
#include <chrono>

bool run_matvec_benchmark(std::ostream& out) {
    ConfigLoader config("config.yaml");
    for (auto ringDim : config.GetMatVecRingDims()) {
        for (auto size : config.GetMatVecMatrixSizes()) {
            // Uncomment the following line to use the function variants from the config file
            for (const auto& function : config.GetMatVecFunctionVariants()) {
                auto start = std::chrono::high_resolution_clock::now();
                bool success = RunMatrixVectorMultiplication(function, ringDim, size, config.GetMatVecMaxMatrixValue(), config.GetMatVecMaxVectorValue());
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
