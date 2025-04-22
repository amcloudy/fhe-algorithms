#include <iostream>

using namespace lbcrypto;

CryptoContext<DCRTPoly> CreateCryptoContext(FHEScheme scheme, uint32_t depth, uint32_t scaleModSize, uint32_t batchSize, uint32_t ringDim, bool enableBootstrapping) {
    if (scheme != FHEScheme::CKKS) {
        std::cerr << "Only CKKS is supported currently.\n";
        std::exit(EXIT_FAILURE);
    }

    CCParams<CryptoContextCKKSRNS> params;
    params.SetMultiplicativeDepth(depth);
    params.SetScalingModSize(scaleModSize);
//    params.SetBatchSize(batchSize);
    params.SetSecurityLevel(SecurityLevel::HEStd_128_classic);
    params.SetRingDim(ringDim);
    params.SetScalingTechnique(FLEXIBLEAUTOEXT);

//    if (enableBootstrapping) {
//        params.SetEnableBootstrapping(true);
//    }

    auto cc = GenCryptoContext(params);

    cc->Enable(PKE);
    cc->Enable(KEYSWITCH);
    cc->Enable(LEVELEDSHE);
    cc->Enable(ADVANCEDSHE);

//    if (enableBootstrapping) {
//        cc->Enable(BOOTSTRAPPING);
//    }
    return cc;
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
        std::cout << "🎚️   Scaling Technique: " << std::setw(18) << static_cast<int>(cryptoParamsCKKS->GetScalingTechnique()) << "\n";
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
