#include "matvec.h"
#include "utils/helper_utils.h"
#include "utils/config_loader.h"
#include "utils/serialization_utils.h"

#include "openfhe.h"
#include <omp.h>

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

//Ciphertext<DCRTPoly> MatrixVectorMultiplicationParallel(const CryptoContext<DCRTPoly> &cc, const std::vector<std::vector<double>> &inputMatrix, const Ciphertext<DCRTPoly> &encInput) {
//    size_t dimension = inputMatrix.size();
//    std::vector<std::vector<double>> diagonalMatrix = ExtractDiagonals(inputMatrix);
//    std::vector<Plaintext> plainDiagonalMatrix(dimension);
//    std::vector<Ciphertext<DCRTPoly>> tempCiphertext(dimension);
//
//    for (size_t i = 0; i < dimension; ++i) {
//        plainDiagonalMatrix[i] = cc->MakeCKKSPackedPlaintext(diagonalMatrix[i]);
//        tempCiphertext[i] = (i > 0) ? cc->EvalRotate(encInput, i) : encInput;
//        tempCiphertext[i] = cc->EvalMult(tempCiphertext[i], plainDiagonalMatrix[i]);
//    }
//    return cc->EvalAddMany(tempCiphertext);
//}

Ciphertext<DCRTPoly> MatrixVectorMultiplicationParallel(const CryptoContext<DCRTPoly> &cc, const std::vector<std::vector<double>> &inputMatrix, const Ciphertext<DCRTPoly> &encInput) {
    size_t dimension = inputMatrix.size();
    std::vector<std::vector<double>> diagonalMatrix = ExtractDiagonals(inputMatrix);

    std::vector<Plaintext> plainDiagonalMatrix(dimension);
    std::vector<Ciphertext<DCRTPoly>> tempCiphertext(dimension);

    // Parallel region for rotation + multiplication
    #pragma omp parallel for
    for (int i = 0; i < static_cast<int>(dimension); ++i) {
        plainDiagonalMatrix[i] = cc->MakeCKKSPackedPlaintext(diagonalMatrix[i]);
        Ciphertext<DCRTPoly> rotatedEncInput = (i > 0) ? cc->EvalRotate(encInput, i) : encInput;
        tempCiphertext[i] = cc->EvalMult(rotatedEncInput, plainDiagonalMatrix[i]);
    }

    // Final reduction (must remain sequential)
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
    // Load parameters from config
    ConfigLoader config("config.yaml");
    auto scheme = ParseScheme(config.GetFHEScheme());
    if (scheme != FHEScheme::CKKS) {
        std::cerr << "Only CKKS is supported currently.\n";
        return false;
    }

    CCParams<CryptoContextCKKSRNS> params;
    params.SetMultiplicativeDepth(config.GetMatVecDepth());
    params.SetScalingModSize(config.GetScaleModSize());
    params.SetSecurityLevel(SecurityLevel::HEStd_128_classic);
    params.SetRingDim(ringDim);
    params.SetScalingTechnique(FLEXIBLEAUTOEXT);

    // Uncomment this if bootstrapping is supported/enabled later
    // if (fheParams.enableBootstrapping) {
    //     params.SetEnableBootstrapping(true);
    // }

    CryptoContext<DCRTPoly> cc = GenCryptoContext(params);
    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);
    // if (fheParams.enableBootstrapping) {
    //     cc->Enable(BOOTSTRAPPING);
    // }

    // Key generation
    auto keys = cc->KeyGen();
    cc->EvalMultKeyGen(keys.secretKey);
    cc->EvalAtIndexKeyGen(keys.secretKey, {1});

    std::vector<int> rotationIndices;
    if (functionType == "parallel") {
        rotationIndices.resize(dim);
        for (size_t i = 0; i < dim; ++i) {
            rotationIndices[i] = i;
        }
    } else {
        rotationIndices = config.GetMatVecRotationIndices();
    }
    cc->EvalRotateKeyGen(keys.secretKey, rotationIndices);

    // SerializeToFile(config.GetKeysPath(), config.GetClientContextFileLocation(), cc);
    // SerializeToFile(config.GetKeysPath(), config.GetServerContextFileLocation(), cc);
    // SerializeToFile(config.GetKeysPath(), config.GetMulKeysFileLocation(), cc, "EvalMult");
    // SerializeToFile(config.GetKeysPath(), config.GetRotKeysFileLocation(), cc, "EvalAutomorphism");
    // SerializeToFile(config.GetKeysPath(), config.GetSecretKeyFileLocation(), keys.secretKey);
    // SerializeToFile(config.GetKeysPath(), config.GetPublicKeyFileLocation(), keys.publicKey);


//    cc->ClearEvalMultKeys();
//    cc->ClearEvalAutomorphismKeys();
//    lbcrypto::CryptoContextFactory<lbcrypto::DCRTPoly>::ReleaseAllContexts();


    // Generate random matrix and vector
    std::vector<std::vector<double>> matrix;
    std::vector<double> vector;
    utils::GenerateRandomMatrix(matrix, dim, maxMatrixVal);
    utils::GenerateRandomVector(vector, dim, maxVectorVal);

    // Encrypt vector

    // Deserialize the crypto context and required keys from the client
//    lbcrypto::CryptoContext<lbcrypto::DCRTPoly> ccServer;
//    DeserializeFromFile(configLoader.GetContextFile(), ccServer);
//    DeserializeFromFile(configLoader.GetMulKeysFile(), ccServer, "EvalMult");
//    DeserializeFromFile(configLoader.GetRotKeysFile(), ccServer, "EvalAutomorphism");
//
//
    utils::PrintContextSummary(cc, scheme);
    std::vector<double> replicatedVector = ReplicateVector(vector);
    auto pt = cc->MakeCKKSPackedPlaintext(replicatedVector);
    auto encVec = cc->Encrypt(keys.publicKey, pt);

    // Compute encrypted matrix-vector multiplication
    Ciphertext<DCRTPoly> encResult;
    if (functionType == "helib") {
        encResult = MatrixVectorMultiplication(cc, matrix, encVec);
    } else if (functionType == "parallel") {
        encResult = MatrixVectorMultiplicationParallel(cc, matrix, encVec);
    } else {
        std::cerr << "❌ Unknown function type: " << functionType << "\n";
        return false;
    }

    // Decrypt and compare with expected result
    Plaintext result;
    cc->Decrypt(keys.secretKey, encResult, &result);
    result->SetLength(vector.size());

    std::vector<double> expected = MatrixVectorMultiplicationExpected(matrix, vector);
    bool isEqual = compare_vectors(result->GetRealPackedValue(), expected);

    auto end = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << "🔢  Matrix dimension:  " << std::setw(18) << dim << "\n";
    std::cout << "⏱️   Total time:     " << std::setw(18) << std::fixed << std::setprecision(2) << ms << " ms\n";
    std::cout << "🔍  Correctness:        " << std::setw(18) << (isEqual ? "✅" : "❌") << "\n\n";

    return isEqual;
}