#include "matvec.h"
#include "utils/helper_utils.h"
#include "utils/config_loader.h"
#include "context.h"
#include "openfhe.h"

#include <chrono>
using Clock = std::chrono::high_resolution_clock;

using namespace lbcrypto;

std::vector<double> ReplicateVector(const std::vector<double> &inputVector) {
    std::vector<double> result(inputVector);
    result.insert(result.end(), inputVector.begin(), inputVector.end() - 1);
    return result;
}

std::vector<std::vector<double>> ExtractDiagonals(const std::vector<std::vector<double>> &inputMatrix) {
    size_t dimension = inputMatrix.size();
    std::vector<std::vector<double>> diagonalMatrix(dimension, std::vector<double>(dimension));
    for (size_t i = 0; i < dimension; ++i) {
        for (size_t j = 0; j < dimension; ++j) {
            diagonalMatrix[i][j] = inputMatrix[j][(i + j) % dimension];
        }
    }
    return diagonalMatrix;
}

Ciphertext<DCRTPoly> MatrixVectorMultiplication(const CryptoContext<DCRTPoly> &cc, const std::vector<std::vector<double>> &inputMatrix, const Ciphertext<DCRTPoly> &encInput) {
    size_t dimension = inputMatrix.size();
    std::vector<std::vector<double>> diagonalMatrix = ExtractDiagonals(inputMatrix);
    std::vector<Plaintext> plainDiagonalMatrix(dimension);
    std::vector<Ciphertext<DCRTPoly>> tempCiphertext(dimension);
    Ciphertext<DCRTPoly> tmp = encInput;

    for (size_t i = 0; i < dimension; ++i) {
        plainDiagonalMatrix[i] = cc->MakeCKKSPackedPlaintext(diagonalMatrix[i]);
        tmp = (i > 0) ? cc->EvalRotate(tmp, 1) : tmp;
        tempCiphertext[i] = cc->EvalMult(tmp, plainDiagonalMatrix[i]);
    }
    return cc->EvalAddMany(tempCiphertext);
}

Ciphertext<DCRTPoly> InnerProduct(const CryptoContext<DCRTPoly> &cc, const Ciphertext<DCRTPoly> &encInput, const std::vector<double> &weight) {
    Plaintext plainWeight = cc->MakeCKKSPackedPlaintext(weight);
    Ciphertext<DCRTPoly> encDotProduct = cc->EvalMult(encInput, plainWeight);
    std::vector<Ciphertext<DCRTPoly>> encRotatedVectors(weight.size());
    Ciphertext<DCRTPoly> tmp = encDotProduct;

    for (size_t i = 0; i < weight.size(); ++i) {
        tmp = (i > 0) ? cc->EvalRotate(tmp, 1) : tmp;
        encRotatedVectors[i] = tmp;
    }

    return cc->EvalAddMany(encRotatedVectors);
}

bool compare_vectors(const std::vector<double>& a, const std::vector<double>& b, double epsilon) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::fabs(a[i] - b[i]) > epsilon) {
            std::cerr << "Mismatch at index " << i << ": expected " << a[i] << ", got " << b[i] << "\n";
            return false;
        }
    }
    return true;
}

std::vector<double> MatrixVectorMultiplicationExpected(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector) {
    std::vector<double> result(matrix.size(), 0.0);
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

bool RunMatrixVectorMultiplication(std::string functionType, uint32_t ringDim, uint32_t dim, double maxMatrixVal, double maxVectorVal) {
    utils::PrintHeader("Running Matrix–Vector Multiplication\n", "⚙️");
    auto start = Clock::now();

    // Shared FHE settings
    ConfigLoader config("config.yaml");
    auto fheParams = config.get_fhe_params();
    auto matvecParams = config.get_matvec_params();

    auto scheme = ParseScheme(fheParams.scheme);
    auto cc = CreateCryptoContext(scheme, matvecParams.depth, fheParams.scaleModSize, dim, ringDim);

    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalAtIndexKeyGen(keys.secretKey, {1});
    cc->EvalRotateKeyGen(keys.secretKey, matvecParams.rotationIndices);

    std::vector<std::vector<double>> matrix;
    std::vector<double> vector;
    utils::GenerateRandomMatrix(matrix, dim, maxMatrixVal);
    utils::GenerateRandomVector(vector, dim, maxVectorVal);

    PrintContextSummary(cc, scheme);
    std::vector<double> replicatedVector = ReplicateVector(vector);
    auto pt = cc->MakeCKKSPackedPlaintext(replicatedVector);
    auto encVec = cc->Encrypt(keys.publicKey, pt);

    Ciphertext<DCRTPoly> encResult;
    if (functionType == "helib") {
        encResult = MatrixVectorMultiplication(cc, matrix, encVec);
    }
    else if (functionType == "custom") {
        std::cout<<"\nCustom function is not implemented yet.\n";
    }
    else {
        std::cerr << "❌ Unknown function type: " << functionType << "\n";
        return false;
    }

    Plaintext result;
    cc->Decrypt(keys.secretKey, encResult, &result);
    result->SetLength(vector.size());

    std::cout << "🔢  Matrix dimension:  " << std::setw(18) << dim << "\n";

    std::vector<double> expected = MatrixVectorMultiplicationExpected(matrix, vector);
    bool isEqual = compare_vectors(result->GetRealPackedValue(), expected);

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "⏱️   Total time:     " << std::setw(18) << std::fixed << std::setprecision(2) << ms << " ms\n";
    if (!isEqual) {
        std::cout << "🔍  Correctness:        " << std::setw(18) << "❌" << "\n\n";
    } else {
        std::cout << "🔍  Correctness:        " << std::setw(18) << "✅" << "\n\n";
    }
    return isEqual;
}
