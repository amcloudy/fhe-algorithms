#ifndef _FHE_NN_H_
#define _FHE_NN_H_

#include "layers.h"
#include "serialization_utils.h"

class FHEClient {
    public:
        lbcrypto::CryptoContext<lbcrypto::DCRTPoly> ccClient;
        lbcrypto::PublicKey<lbcrypto::DCRTPoly> publicKey;
        lbcrypto::PrivateKey<lbcrypto::DCRTPoly> secretKey;

        explicit FHEClient();

        bool DeserializeKeys();
        lbcrypto::Ciphertext<lbcrypto::DCRTPoly> PrepareInput(const std::vector<double>& inputVector) const;
        double ComputePlainResult(const lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& encResult) const;
};

class FHEServer {
    public:
        lbcrypto::CryptoContext<lbcrypto::DCRTPoly> context;

        explicit FHEServer(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly> &cc);

        lbcrypto::Ciphertext<lbcrypto::DCRTPoly> ComputeEncResult(const lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& encInput) const;
};

#endif //_FHE_NN_H_
