#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "scheme_defs.h"

FHEScheme ParseScheme(const std::string& schemeStr);

class ConfigLoader {
public:
    struct FHEParams {
        std::string scheme;
        uint32_t scaleModSize;
        bool enableBootstrapping;
        std::string contextFileLocation;
        std::string secretKeyFileLocation;
        std::string publicKeyFileLocation;
        std::string rotKeysFileLocation;
        std::string mulKeysFileLocation;
        std::string keysPath;
    };

    struct MatVecParams {
        std::vector<uint32_t> ringDims;
        uint32_t depth;
        std::vector<uint32_t> matrixSizes;
        double max_matrix_value;
        double max_vector_value;
        std::vector<int> rotationIndices;
        std::string outputCSV;
        std::vector<std::string> functionVariants;
    };

    struct PolyEvalParams {
        std::vector<uint32_t> ringDims;
        uint32_t depth;
        std::vector<uint32_t> vectorSizes;
        double max_vector_value;
        std::string outputCSV;
        std::vector<double> chebyshevCoeff;
        std::vector<std::string> functionVariants;
    };

    explicit ConfigLoader(const std::string& yamlPath);

    const FHEParams& get_fhe_params() const { return fhe; }
    const MatVecParams& get_matvec_params() const { return matvec; }
    const PolyEvalParams& get_polyeval_params() const { return polyeval; }

private:
    FHEParams fhe;
    MatVecParams matvec;
    PolyEvalParams polyeval;
    void load_config(const std::string& yamlPath);
};

#endif  // CONFIG_LOADER_H
