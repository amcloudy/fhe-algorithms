#ifndef POLY_EVAL_H
#define POLY_EVAL_H

#include "openfhe.h"
#include "context.h"
#include "utils/helper_utils.h"
#include "utils/config_loader.h"

using namespace lbcrypto;

/**
 * @brief Compute odd powers of an encrypted value using balanced multiplication tree.
 */
std::vector<Ciphertext<DCRTPoly>> compute_odd_powers(const CryptoContext<DCRTPoly>& cc,
                                                     const Ciphertext<DCRTPoly>& enc_input,
                                                     size_t degree);

/**
 * @brief Evaluate Chebyshev polynomial homomorphically using odd-degree terms.
 */
Ciphertext<DCRTPoly> chebyshev_polynomial_evaluation(const CryptoContext<DCRTPoly>& cc,
                                                     const Ciphertext<DCRTPoly>& enc_input);

/**
 * @brief Run polynomial evaluation demo with either EvalPoly or manual Chebyshev method.
 */
bool RunPolynomialEvaluation(std::string functionType, uint32_t ringDim, uint32_t vecDim, double maxVectorVal);


#endif // POLY_EVAL_H
