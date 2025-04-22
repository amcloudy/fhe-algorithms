#include "config_loader.h"
#include <stdexcept>

FHEScheme ParseScheme(const std::string& schemeStr) {
    if (schemeStr == "ckks") return FHEScheme::CKKS;
    if (schemeStr == "bfv")  return FHEScheme::BFV;
    if (schemeStr == "bgv")  return FHEScheme::BGV;
    throw std::invalid_argument("Unsupported scheme: " + schemeStr);
}

ConfigLoader::ConfigLoader(const std::string& yamlPath) {
    load_config(yamlPath);
}

void ConfigLoader::load_config(const std::string& yamlPath) {
    YAML::Node config = YAML::LoadFile(yamlPath);

    // ------------------ FHE Params ------------------
    const auto& fheNode = config["fhe"];
    fhe.scheme = fheNode["scheme"].as<std::string>();
    fhe.scaleModSize = fheNode["scale_mod_size"].as<uint32_t>();
    fhe.enableBootstrapping = fheNode["enable_bootstrapping"].as<bool>();
    fhe.keysPath = fheNode["keyLocation"].as<std::string>();
    fhe.contextFileLocation = fhe.keysPath + fheNode["contextFile"].as<std::string>();
    fhe.secretKeyFileLocation = fhe.keysPath + fheNode["secretKeyFile"].as<std::string>();
    fhe.publicKeyFileLocation = fhe.keysPath + fheNode["publicKeyFile"].as<std::string>();
    fhe.rotKeysFileLocation = fhe.keysPath + fheNode["rotKeysFile"].as<std::string>();
    fhe.mulKeysFileLocation = fhe.keysPath + fheNode["mulKeysFile"].as<std::string>();

    // ------------------ MatVec Params ------------------
    const auto& matvecNode = config["matvec"];
    matvec.ringDims = matvecNode["ringDims"].as<std::vector<uint32_t>>();
    matvec.depth = matvecNode["depth"].as<uint32_t>();
    matvec.matrixSizes = matvecNode["matrixSizes"].as<std::vector<uint32_t>>();
    matvec.max_matrix_value = matvecNode["max_matrix_value"].as<double>();
    matvec.max_vector_value = matvecNode["max_vector_value"].as<double>();
    matvec.rotationIndices = matvecNode["rotation_indices"].as<std::vector<int>>();
    matvec.outputCSV = matvecNode["outputCSV"].as<std::string>();
    matvec.functionVariants = matvecNode["functionVariants"].as<std::vector<std::string>>();

    // ------------------ PolyEval Params ------------------
    const auto& polyNode = config["polyeval"];
    polyeval.ringDims = polyNode["ringDims"].as<std::vector<uint32_t>>();
    polyeval.depth = polyNode["depth"].as<uint32_t>();
    polyeval.vectorSizes = polyNode["vectorSizes"].as<std::vector<uint32_t>>();
    polyeval.max_vector_value = polyNode["max_vector_value"].as<double>();
    polyeval.outputCSV = polyNode["outputCSV"].as<std::string>();
    polyeval.chebyshevCoeff = polyNode["chebyshevCoeff"].as<std::vector<double>>();
    polyeval.functionVariants = polyNode["functionVariants"].as<std::vector<std::string>>();
}
