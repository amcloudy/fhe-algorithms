#ifndef _MAT_VEC_H_
#define _MAT_VEC_H_

#include <vector>
#include <iostream>
#include "openfhe.h"
#include "utils/config_loader.h"
#include "utils/helper_utils.h"

using namespace lbcrypto;

std::vector<double> ReplicateVector(const std::vector<double>& inputVector);
std::vector<std::vector<double>> ExtractDiagonals(const std::vector<std::vector<double>>& inputMatrix);
Ciphertext<DCRTPoly> MatrixVectorMultiplication(const CryptoContext<DCRTPoly>& cc, const std::vector<std::vector<double>>& inputMatrix, const Ciphertext<DCRTPoly>& encInput);
Ciphertext<DCRTPoly> MatrixVectorMultiplicationParallel(const CryptoContext<DCRTPoly>& cc, const std::vector<std::vector<double>>& inputMatrix, const Ciphertext<DCRTPoly>& encInput);
Ciphertext<DCRTPoly> InnerProduct(const CryptoContext<DCRTPoly>& cc, const Ciphertext<DCRTPoly>& encInput, const std::vector<double>& weight);
bool compare_vectors(const std::vector<double>& a, const std::vector<double>& b, double epsilon = 0.01);
std::vector<double> MatrixVectorMultiplicationExpected(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector);
bool RunMatrixVectorMultiplication(std::string functionType, uint32_t ringDim, uint32_t matDim, double maxMatrixVal = 0, double maxVectorVal = 0);

#endif  // _MAT_VEC_H_
