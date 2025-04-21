#include <iostream>
#include <limits>
#include "matvec.h"
#include "poly_eval.h"
#include "utils/helper_utils.h"

void RunMatvecDemo();
void RunPolyEvalDemo();

int main() {
    utils::PrintHeader("FHE Algorithms Demo Menu", "🔐");

    while (true) {
        std::cout << "----------------------------\n";
        std::cout << " [1] Matrix–Vector Multiplication\n";
        std::cout << " [2] Polynomial Evaluation\n";
        std::cout << " [0] Exit\n";
        std::cout << "Choose an option: ";
        std::cout.flush();

        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cerr << "❌ Input stream closed or error occurred. Exiting.\n";
            break;
        }

        int choice;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cerr << "❌ Invalid input. Please enter a number.\n";
            continue;
        }
        switch (choice) {
            case 1: RunMatvecDemo(); break;
            case 2: RunPolyEvalDemo(); break;
            case 0:
                std::cout << "👋 Exiting...\n";
            return 0;
            default:
                std::cerr << "❌ Invalid choice. Try again.\n";
        }
        std::cout << "\n";
    }
    return 0;
}
