#include "serialization_utils.h"

#include <filesystem>
#include "openfhe.h"
#include "scheme/ckksrns/ckksrns-ser.h"
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "config_loader.h"

void SerializeToFile(const std::string& dirname, const std::string& filename, const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, const std::string& keyType)
{
    EnsureDirectory(dirname);
    RemoveFileIfExists(filename);

    std::ofstream fileOut(filename, std::ios::binary);
    if (!fileOut) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    if (keyType == "EvalMult") {
        if (!cc->SerializeEvalMultKey(fileOut, lbcrypto::SerType::BINARY)) {
            throw std::runtime_error("Error serializing EvalMult key to " + filename);
        }
    }
    else if (keyType == "EvalAutomorphism") {
        if (!cc->SerializeEvalAutomorphismKey(fileOut, lbcrypto::SerType::BINARY)) {
            throw std::runtime_error("Error serializing EvalAutomorphism key to " + filename);
        }
    }
    else {
        throw std::invalid_argument("Invalid key type: " + keyType);
    }
}

void DeserializeFromFile(const std::string& filePath, const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, const std::string& keyType)
{
    std::ifstream fileIn(filePath, std::ios::binary);
    if (!fileIn) {
        throw std::runtime_error("Could not open file for deserialization: " + filePath);
    }

    if (keyType == "EvalMult") {
        if (!cc->DeserializeEvalMultKey(fileIn, lbcrypto::SerType::BINARY)) {
            throw std::runtime_error("Error deserializing EvalMult key from " + filePath);
        }
    }
    else if (keyType == "EvalAutomorphism") {
        if (!cc->DeserializeEvalAutomorphismKey(fileIn, lbcrypto::SerType::BINARY)) {
            throw std::runtime_error("Error deserializing EvalAutomorphism key from " + filePath);
        }
    }
    else {
        throw std::invalid_argument("Invalid key type: " + keyType);
    }
}
