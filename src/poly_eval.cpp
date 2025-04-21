#include "poly_eval.h"
#include "utils/helper_utils.h"
#include "utils/config_loader.h"
#include "openfhe.h"
#include "matvec.h"
#include <chrono>

#include <chrono>
using Clock = std::chrono::high_resolution_clock;

using namespace lbcrypto;

std::vector<double> PolynomialEvaluationExpected(const std::vector<double>& vector, const std::vector<double>& coeffs);

std::vector<Ciphertext<DCRTPoly>> compute_odd_powers(const CryptoContext<DCRTPoly>& cc, const Ciphertext<DCRTPoly>& enc_input, size_t degree) {
    std::vector<Ciphertext<DCRTPoly>> powers(degree + 1);
    powers[1] = enc_input;

    std::vector<int> levels(degree + 1, 0);
    for (int i = 2; i <= static_cast<int>(degree); i++) {
        int min_level = i;
        int candidate = -1;
        for (int j = 1; j <= i / 2; j++) {
            int k = i - j;
            int new_level = std::max(levels[j], levels[k]) + 1;
            if (new_level < min_level) {
                candidate = j;
                min_level = new_level;
            }
        }
        if (candidate < 0)
            throw std::runtime_error("Invalid candidate in encrypted power computation");
        levels[i] = min_level;
        powers[i] = cc->EvalMult(powers[candidate], powers[i - candidate]);
    }

    std::vector<Ciphertext<DCRTPoly>> odd_powers((degree + 1) / 2);
    for (size_t i = 0; i < odd_powers.size(); ++i)
        odd_powers[i] = powers[2 * i + 1];

    return odd_powers;
}

Ciphertext<DCRTPoly> chebyshev_polynomial_evaluation(const CryptoContext<DCRTPoly>& cc, const Ciphertext<DCRTPoly>& enc_input) {
    ConfigLoader config("config.yaml");
    std::vector<double> coeffs = config.get_polyeval_params().chebyshevCoeff;
    size_t coeff_size = coeffs.size();
    std::vector<Ciphertext<DCRTPoly>> powers = compute_odd_powers(cc, enc_input, coeff_size - 1);
    Ciphertext<DCRTPoly> enc_result = cc->EvalMult(powers[0], coeffs[1]);

    for (size_t i = 1; i < coeff_size / 2; i++) {
        auto term = cc->EvalMult(powers[i], coeffs[2 * i + 1]);
        enc_result = cc->EvalAdd(enc_result, term);
    }
    return enc_result;
}

bool RunPolynomialEvaluation(std::string functionType, uint32_t ringDim, uint32_t vecDim, double maxVectorVal) {
    utils::PrintHeader("Running Polynomial Evaluation\n", "⚙️");
    auto start = std::chrono::high_resolution_clock::now();
    ConfigLoader config("config.yaml");

    auto fheParams = config.get_fhe_params();
    auto polyParams = config.get_polyeval_params();
    auto scheme = ParseScheme(fheParams.scheme);

    auto cc = CreateCryptoContext(scheme, polyParams.depth, fheParams.scaleModSize, vecDim, ringDim);

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalAtIndexKeyGen(keys.secretKey, {1});

    std::vector<double> vector;
    utils::GenerateRandomVector(vector, vecDim, maxVectorVal);
    PrintContextSummary(cc, scheme);

    auto pt = cc->MakeCKKSPackedPlaintext(vector);
    auto encVec = cc->Encrypt(keys.publicKey, pt);

    Ciphertext<DCRTPoly> encResult;

    if (functionType == "openfhe") {
        encResult = cc->EvalPoly(encVec, config.get_polyeval_params().chebyshevCoeff);
    }
    else if (functionType == "custom") {
        encResult = chebyshev_polynomial_evaluation(cc, encVec);
    }
    else {
        std::cerr << "❌ Unknown function type: " << functionType << "\n";
        return false;
    }

    Plaintext result;
    cc->Decrypt(keys.secretKey, encResult, &result);
    result->SetLength(vecDim);

    std::cout << "🔢  Vector dimension:  " << std::setw(18) << vecDim << "\n";

    std::vector<double> expected = PolynomialEvaluationExpected(vector, config.get_polyeval_params().chebyshevCoeff);
    bool isEqual = compare_vectors(result->GetRealPackedValue(), expected);
    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "⏱️   Total time:     " << std::setw(18) << std::fixed << std::setprecision(2) << ms << " ms\n";
    std::cout << "🔍  Coorectness:        " << std::setw(18) << (isEqual ? "✅" : "❌") << "\n";

    return isEqual;
}

std::vector<double> PolynomialEvaluationExpected(const std::vector<double>& vector, const std::vector<double>& coeffs) {
    std::vector<double> result(vector.size(), 0.0);
    for (size_t i = 0; i < vector.size(); ++i) {
        for (size_t j = 0; j < coeffs.size(); ++j)
            result[i] += coeffs[j] * std::pow(vector[i], j);
    }
    return result;
}
