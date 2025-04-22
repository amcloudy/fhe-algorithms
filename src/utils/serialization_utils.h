#ifndef SERIALIZATION_UTILS_H
#define SERIALIZATION_UTILS_H

#include <filesystem>
#include "openfhe.h"
#include "scheme/ckksrns/ckksrns-ser.h"
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "key/key-ser.h"
#include "config_loader.h"

void SerializeToFile(const std::string& dirname, const std::string& filename, const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, const std::string& keyType);
void DeserializeFromFile(const std::string& filePath, const lbcrypto::CryptoContext<lbcrypto::DCRTPoly>& cc, const std::string& keyType);

inline void EnsureDirectory(const std::filesystem::path& dirname) {
    std::error_code ec;
    if (!std::filesystem::exists(dirname)) {
        std::filesystem::create_directories(dirname, ec);
        if (ec) {
            throw std::runtime_error("Failed to create directory: " + dirname.string() + " (" + ec.message() + ")");
        }
    }
}

inline void RemoveFileIfExists(const std::filesystem::path& filename) {
    std::error_code ec;
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename, ec);
        if (ec) {
            throw std::runtime_error("Failed to remove file: " + filename.string() + " (" + ec.message() + ")");
        }
    }
}

template<typename T>
inline void SerializeToFile(const std::string& dirname, const std::string& filename, const T& data) {
    EnsureDirectory(dirname);
    RemoveFileIfExists(filename);

    if (!lbcrypto::Serial::SerializeToFile(filename, data, lbcrypto::SerType::BINARY)) {
        throw std::runtime_error("Error writing serialized data to " + filename);
    }
}

template<typename T>
inline void DeserializeFromFile(const std::string& filename, T& data) {
    if (!lbcrypto::Serial::DeserializeFromFile(filename, data, lbcrypto::SerType::BINARY)) {
        throw std::runtime_error("Error deserializing data from " + filename);
    }
}

#endif //SERIALIZATION_UTILS_H
