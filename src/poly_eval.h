#ifndef POLY_EVAL_H
#define POLY_EVAL_H

#include "openfhe.h"
#include "utils/helper_utils.h"
#include "utils/config_loader.h"

using namespace lbcrypto;

std::vector<Ciphertext<DCRTPoly>> compute_odd_powers(const CryptoContext<DCRTPoly>& cc, const Ciphertext<DCRTPoly>& enc_input, size_t degree);
Ciphertext<DCRTPoly> chebyshev_polynomial_evaluation(const CryptoContext<DCRTPoly>& cc, const Ciphertext<DCRTPoly>& enc_input);
bool RunPolynomialEvaluation(std::string functionType, uint32_t ringDim, uint32_t vecDim, double maxVectorVal);

#endif // POLY_EVAL_H
