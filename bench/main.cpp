#include "../src/utils/config_loader.h"
#include <iostream>
#include <fstream>
#include <functional>

bool run_matvec_benchmark(const ConfigLoader::MatVecParams&, std::ostream&);
bool run_polyeval_benchmark_func(const ConfigLoader::PolyEvalParams&, std::ostream&);

int main() {
    ConfigLoader config(CONFIG_PATH);

    while (true) {
        std::cout << "\n============================\n";
        std::cout << "🔬 FHE Algorithm Benchmarks\n";
        std::cout << "============================\n";
        std::cout << " [1] Matrix–Vector Multiplication\n";
        std::cout << " [2] Polynomial Evaluation\n";
        std::cout << " [0] Exit\n";
        std::cout << "Choose an option: ";
        std::cout.flush();

        std::string input;
        if (!std::getline(std::cin, input)) break;

        int choice = -1;
        try { choice = std::stoi(input); } catch (...) {}

        switch (choice) {
            case 1: {
                const auto& cfg = config.get_matvec_params();
                std::ofstream out(cfg.outputCSV);
                out << "ring_dim,matrix_size,time_ms\n";
                if (run_matvec_benchmark(cfg, out))
                    std::cout << "✅ Matvec benchmark done: " << cfg.outputCSV << "\n";
                else
                    std::cerr << "❌ Matvec benchmark failed.\n";
                break;
            }

            case 2: {
                const auto& cfg = config.get_polyeval_params();

                std::ofstream out(cfg.outputCSV, std::ios::trunc);  // ✅ clear file
                if (!out.is_open()) {
                    std::cerr << "❌ Failed to open output file: " << cfg.outputCSV << "\n";
                    break;
                }

                out << "ring_dim, vecSize, function, time_ms\n";

                if (run_polyeval_benchmark_func(cfg, out))
                    std::cout << "✅ PolyEval benchmark done: " << cfg.outputCSV << "\n";
                else
                    std::cerr << "❌ PolyEval benchmark failed.\n";

                out.close();
                break;
            }

            case 0:
                std::cout << "👋 Exiting...\n";
                return 0;

            default:
                std::cerr << "❌ Invalid choice.\n";
        }
    }

    return 0;
}
