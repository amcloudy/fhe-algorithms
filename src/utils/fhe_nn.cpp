#include "../include/fhe_nn.h"

FHEClient::FHEClient() {
    if (!DeserializeKeys()) {
        std::cerr << "Deserialization failed!" << std::endl;
    }
}

bool FHEClient::DeserializeKeys() {
    ConfigLoader configLoader("config.yaml");
    bool success = false;
    try {
        DeserializeFromFile(configLoader.GetPublicKeyFile(), publicKey);
        DeserializeFromFile(configLoader.GetSecretKeyFile(), secretKey);
        DeserializeFromFile(configLoader.GetClientContextFile(), ccClient);
        success = true;
    } catch (const std::exception& e) {
        std::cerr << "Error during deserialization: " << e.what() << std::endl;
    }
    return success;
}

lbcrypto::Ciphertext<lbcrypto::DCRTPoly> FHEClient::PrepareInput(const std::vector<double>& inputVector) const {
    return PrepareClientData(ccClient, publicKey, inputVector);
}

double FHEClient::ComputePlainResult(const lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& encInput) const {
    lbcrypto::Plaintext fheResultPlain = ComputeResult(ccClient, secretKey, encInput);
    return fheResultPlain->GetRealPackedValue()[0];
}

FHEServer::FHEServer(const lbcrypto::CryptoContext<lbcrypto::DCRTPoly> &cc) : context(cc) {}

lbcrypto::Ciphertext<lbcrypto::DCRTPoly> FHEServer::ComputeEncResult(const lbcrypto::Ciphertext<lbcrypto::DCRTPoly>& encInput) const {
    return ComputeEncryptedResult(context, encInput);
}
