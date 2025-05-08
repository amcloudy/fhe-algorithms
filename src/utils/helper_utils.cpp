#include "helper_utils.h"
using namespace lbcrypto;

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

    void PrintContextSummary(const CryptoContext<DCRTPoly>& cc, FHEScheme scheme) {
        switch (scheme) {
            case FHEScheme::CKKS: std::cout << "📚  Scheme:            " << std::setw(18) << "CKKS" << "\n"; break;
            case FHEScheme::BFV:  std::cout << "📚  Scheme:            " << std::setw(18) << "BFV" << "\n"; break;
            case FHEScheme::BGV:  std::cout << "📚  Scheme:            " << std::setw(18) << "BGV" << "\n"; break;
        }

        auto encodingParams = cc->GetEncodingParams();
        auto elemParams = cc->GetCryptoParameters()->GetElementParams();
        std::cout << "📏  Ring Dimension:    " << std::setw(18) << elemParams->GetRingDimension() << "\n";
    //    std::cout << "🔢  Batch Size:        " << std::setw(18) << encodingParams->GetBatchSize() << "\n";

        auto cryptoParamsCKKS = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
        if (cryptoParamsCKKS) {
            auto secLevel = cryptoParamsCKKS->GetStdLevel();
            std::string levelStr =
                (secLevel == SecurityLevel::HEStd_128_classic) ? "HEStd_128_classic" :
                (secLevel == SecurityLevel::HEStd_192_classic) ? "HEStd_192_classic" :
                (secLevel == SecurityLevel::HEStd_256_classic) ? "HEStd_256_classic" :
                "Unknown";

            std::cout << "🔒  Security Level:    " << std::setw(18) << levelStr << "\n";
            std::cout << "🎚️ Scaling Technique: " << std::setw(18) << static_cast<int>(cryptoParamsCKKS->GetScalingTechnique()) << "\n";
        } else {
            std::cout << "⚠️ CKKS-specific parameters unavailable.\n";
        }

        // std::cout << "Bootstrapping Enabled: "
        //           << (cc->IsFeatureEnabled(PKESchemeFeature::BOOTSTRAPPING) ? "Yes" : "No") << "\n";
    }

    void PrintModulusChain(const CryptoContext<DCRTPoly>& cc) {
        std::cout << "\n[Modulus Chain Details]\n";

        auto paramsQ = cc->GetElementParams()->GetParams();
        std::cout << "Moduli in Q:\n";
        for (uint32_t i = 0; i < paramsQ.size(); i++) {
            std::cout << "  q" << i << ": " << paramsQ[i]->GetModulus() << "\n";
        }

        auto cryptoParamsCKKS = std::dynamic_pointer_cast<CryptoParametersCKKSRNS>(cc->GetCryptoParameters());
        if (!cryptoParamsCKKS) {
            std::cout << "P moduli unavailable (non-CKKS scheme).\n";
            return;
        }

        auto paramsQP = cryptoParamsCKKS->GetParamsQP();
        const auto& allParams = paramsQP->GetParams();

        BigInteger P(1);
        std::cout << "Moduli in P:\n";
        for (uint32_t i = paramsQ.size(); i < allParams.size(); i++) {
            auto mod = allParams[i]->GetModulus();
            std::cout << "  p" << (i - paramsQ.size()) << ": " << mod << "\n";
            P *= mod;
        }
        // std::cout << "Product of P moduli (P): " << P << "\n";
    }

} // namespace utils