#pragma once

#include "openfhe.h"
#include <memory>
#include <string>
#include <iomanip>

#include "utils/config_loader.h"
#include "utils/scheme_defs.h"

/**
 * @brief Create a CryptoContext from FHE + algorithm-specific config
 */
lbcrypto::CryptoContext<lbcrypto::DCRTPoly>
CreateCryptoContext(FHEScheme scheme,
                    uint32_t depth,
                    uint32_t scaleModSize,
                    uint32_t batchSize,
                    uint32_t ringDim,
                    bool enableBootstrapping = false);

/**
 * @brief Print summary of the CryptoContext config (scheme, batch size, security, etc.)
 */
void PrintContextSummary(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, FHEScheme scheme);

/**
 * @brief Print modulus chain (Q + P moduli, and product of P)
 */
void PrintModulusChain(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc);
