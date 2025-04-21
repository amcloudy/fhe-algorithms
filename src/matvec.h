#ifndef _MAT_VEC_H_
#define _MAT_VEC_H_

#include <vector>
#include <iostream>
#include "openfhe.h"

#include "context.h"
#include "utils/config_loader.h"
#include "utils/helper_utils.h"

using namespace lbcrypto;

/**
 * @brief Replicate a vector to double its size minus one (for testing or alignment).
 */
std::vector<double> ReplicateVector(const std::vector<double>& inputVector);

/**
 * @brief Extract diagonals for diagonal matrix representation of matrix-vector multiplication.
 */
std::vector<std::vector<double>> ExtractDiagonals(const std::vector<std::vector<double>>& inputMatrix);

/**
 * @brief Perform encrypted matrix-vector multiplication using diagonal encoding and rotation.
 */
Ciphertext<DCRTPoly> MatrixVectorMultiplication(const CryptoContext<DCRTPoly>& cc,
                                                const std::vector<std::vector<double>>& inputMatrix,
                                                const Ciphertext<DCRTPoly>& encInput);

/**
 * @brief Compute encrypted inner product between ciphertext and plaintext vector.
 */
Ciphertext<DCRTPoly> InnerProduct(const CryptoContext<DCRTPoly>& cc,
                                  const Ciphertext<DCRTPoly>& encInput,
                                  const std::vector<double>& weight);

/**
 * @brief Compare two real-valued vectors element-wise with a given precision.
 */
bool compare_vectors(const std::vector<double>& a, const std::vector<double>& b, double epsilon = 0.01);

/**
 * @brief Ground-truth matrix-vector multiplication for result validation.
 */
std::vector<double> MatrixVectorMultiplicationExpected(const std::vector<std::vector<double>>& matrix,
                                                       const std::vector<double>& vector);

/**
 * @brief Execute the full matrix-vector multiplication pipeline and validate correctness.
 *
 * @return true if decrypted result is close to expected output.
 */
bool RunMatrixVectorMultiplication(std::string functionType, uint32_t ringDim, uint32_t matDim,
                                       double maxMatrixVal = 0, double maxVectorVal = 0);

#endif  // _MAT_VEC_H_
