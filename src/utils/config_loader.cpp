#include "config_loader.h"
#include <stdexcept>

FHEScheme ParseScheme(const std::string& schemeStr) {
    if (schemeStr == "ckks") return FHEScheme::CKKS;
    if (schemeStr == "bfv")  return FHEScheme::BFV;
    if (schemeStr == "bgv")  return FHEScheme::BGV;
    throw std::invalid_argument("Unsupported scheme: " + schemeStr);
}

ConfigLoader::ConfigLoader(const std::string& configFile) {
    LoadConfiguration(configFile);
}

void ConfigLoader::LoadConfiguration(const std::string& configFile)
{
    YAML::Node config = YAML::LoadFile(configFile);

    // Load FHE parameters
    fheParams.scheme = config["fhe"]["scheme"].as<std::string>();
    fheParams.scaleModSize = config["fhe"]["scale_mod_size"].as<uint32_t>();
    fheParams.enableBootstrapping = config["fhe"]["enable_bootstrapping"].as<bool>();

    // Load matvec parameters
    matVecParams.ringDims = config["matvec"]["ringDims"].as<std::vector<uint32_t>>();
    matVecParams.depth = config["matvec"]["depth"].as<uint32_t>();
    matVecParams.matrixSizes = config["matvec"]["matrixSizes"].as<std::vector<uint32_t>>();
    matVecParams.max_matrix_value = config["matvec"]["max_matrix_value"].as<double>();
    matVecParams.max_vector_value = config["matvec"]["max_vector_value"].as<double>();
    matVecParams.rotationIndices = config["matvec"]["rotation_indices"].as<std::vector<int>>();
    matVecParams.outputCSV = config["matvec"]["outputCSV"].as<std::string>();
    matVecParams.functionVariants = config["matvec"]["functionVariants"].as<std::vector<std::string>>();

    // Load polyeval parameters
    polyEvalParams.ringDims = config["polyeval"]["ringDims"].as<std::vector<uint32_t>>();
    polyEvalParams.depth = config["polyeval"]["depth"].as<uint32_t>();
    polyEvalParams.vectorSizes = config["polyeval"]["vectorSizes"].as<std::vector<uint32_t>>();
    polyEvalParams.max_vector_value = config["polyeval"]["max_vector_value"].as<double>();
    polyEvalParams.outputCSV = config["polyeval"]["outputCSV"].as<std::string>();
    polyEvalParams.chebyshevCoeff = config["polyeval"]["chebyshevCoeff"].as<std::vector<double>>();
    polyEvalParams.functionVariants = config["polyeval"]["functionVariants"].as<std::vector<std::string>>();

    // Load File Paths
    paths.keysPath = config["fhe"]["keyLocation"].as<std::string>();
    paths.contextClientFileLocation = paths.keysPath + config["fhe"]["clientContextFile"].as<std::string>();
    paths.contextServerFileLocation = paths.keysPath + config["fhe"]["serverContextFile"].as<std::string>();
    paths.secretKeyFileLocation = paths.keysPath + config["fhe"]["secretKeyFile"].as<std::string>();
    paths.publicKeyFileLocation = paths.keysPath + config["fhe"]["publicKeyFile"].as<std::string>();
    paths.rotKeysFileLocation = paths.keysPath + config["fhe"]["rotKeysFile"].as<std::string>();
    paths.mulKeysFileLocation = paths.keysPath + config["fhe"]["mulKeysFile"].as<std::string>();
}
