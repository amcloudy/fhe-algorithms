#include "helper_utils.h"

namespace utils {

	void PrintHeader(const std::string& title, const std::string& emoji) {
        std::cout <<"\n"<< emoji << " " << title << "\n";
    }

    void PrintMatrix(const std::vector<std::vector<double>>& matrix) {
        constexpr int cellWidth = 9;
        const std::string hor = u8"─────────";

        std::cout << "┌";
        for (size_t i = 0; i < matrix[0].size(); ++i) std::cout << hor;
        std::cout << "┐\n";

        for (const auto& row : matrix) {
            std::cout << "│";
            for (auto val : row)
                std::cout << std::setw(cellWidth - 1) << std::fixed << std::setprecision(4) << val << " ";
            std::cout << "│\n";
        }

        std::cout << "└";
        for (size_t i = 0; i < matrix[0].size(); ++i) std::cout << hor;
        std::cout << "┘\n";
    }

    void PrintVector(const std::vector<double>& vec) {
        constexpr int cellWidth = 9;
        const std::string hor = u8"─────────";

        std::cout << "┌";
        for (size_t i = 0; i < vec.size(); ++i) std::cout << hor;
        std::cout << "┐\n│";

        for (double val : vec)
            std::cout << std::setw(cellWidth - 1) << std::fixed << std::setprecision(4) << val << " ";
        std::cout << "│\n└";

        for (size_t i = 0; i < vec.size(); ++i) std::cout << hor;
        std::cout << "┘\n";
    }

    void GenerateRandomMatrix(std::vector<std::vector<double>>& matrix, size_t dim, double maxVal) {
        if (maxVal == 0.0) {
            std::cerr << "[⚠️ Warning] maxMatrixVal is 0 — matrix will be all zeros.\n";
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-maxVal, maxVal);

        matrix.resize(dim, std::vector<double>(dim));
        for (size_t i = 0; i < dim; ++i)
            for (size_t j = 0; j < dim; ++j)
                matrix[i][j] = dis(gen);
    }

    void GenerateRandomVector(std::vector<double>& vector, size_t dim, double maxVal) {
        if (maxVal == 0.0) {
            std::cerr << "[⚠️ Warning] maxVectorVal is 0 — vector will be all zeros.\n";
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-maxVal, maxVal);

        vector.resize(dim);
        for (size_t i = 0; i < dim; ++i)
            vector[i] = dis(gen);
    }
} // namespace utils