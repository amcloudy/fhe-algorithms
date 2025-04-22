#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "openfhe.h"
#include <memory>
#include <string>
#include <iomanip>

#include "utils/config_loader.h"
#include "utils/scheme_defs.h"

lbcrypto::CryptoContext<lbcrypto::DCRTPoly> CreateCryptoContext(FHEScheme scheme, uint32_t depth, uint32_t scaleModSize, uint32_t batchSize, uint32_t ringDim, bool enableBootstrapping = false);
void PrintContextSummary(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, FHEScheme scheme);
void PrintModulusChain(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc);

#endif  // _CONTEXT_H_