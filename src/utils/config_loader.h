#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
#include "scheme_defs.h"

FHEScheme ParseScheme(const std::string& schemeStr);

class ConfigLoader {
    private:
        struct FHEParams {
            std::string scheme;
            uint32_t scaleModSize;
            bool enableBootstrapping;
        };
        FHEParams fheParams;

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
        MatVecParams matVecParams;

        struct PolyEvalParams {
            std::vector<uint32_t> ringDims;
            uint32_t depth;
            std::vector<uint32_t> vectorSizes;
            double max_vector_value;
            std::string outputCSV;
            std::vector<double> chebyshevCoeff;
            std::vector<std::string> functionVariants;
        };
        PolyEvalParams polyEvalParams;

        struct FilePaths {
          std::string contextClientFileLocation;
          std::string contextServerFileLocation;
          std::string secretKeyFileLocation;
          std::string publicKeyFileLocation;
          std::string rotKeysFileLocation;
          std::string mulKeysFileLocation;
          std::string keysPath;
        };
        FilePaths paths;

        // Internal method to load configuration
        void LoadConfiguration(const std::string& configFile);

    public:
        explicit ConfigLoader(const std::string& configFile);

        // Accessors for FHE parameters
        std::string GetFHEScheme() const { return fheParams.scheme; }
        uint32_t GetScaleModSize() const { return fheParams.scaleModSize; }
        bool IsBootstrappingEnabled() const { return fheParams.enableBootstrapping; }

        // Accessors for matvec parameters
        std::vector<uint32_t> GetMatVecRingDims() const { return matVecParams.ringDims; }
        uint32_t GetMatVecDepth() const { return matVecParams.depth; }
        std::vector<uint32_t> GetMatVecMatrixSizes() const { return matVecParams.matrixSizes; }
        double GetMatVecMaxMatrixValue() const { return matVecParams.max_matrix_value; }
        double GetMatVecMaxVectorValue() const { return matVecParams.max_vector_value; }
        std::vector<int> GetMatVecRotationIndices() const { return matVecParams.rotationIndices; }
        std::string GetMatVecOutputCSV() const { return matVecParams.outputCSV; }
        std::vector<std::string> GetMatVecFunctionVariants() const { return matVecParams.functionVariants; }

        // Accessors for poly eval parameters
        std::vector<uint32_t> GetPolyEvalRingDims() const { return polyEvalParams.ringDims; }
        uint32_t GetPolyEvalDepth() const { return polyEvalParams.depth; }
        std::vector<uint32_t> GetPolyEvalVectorSizes() const { return polyEvalParams.vectorSizes; }
        double GetPolyEvalMaxVectorValue() const { return polyEvalParams.max_vector_value; }
        std::string GetPolyEvalOutputCSV() const { return polyEvalParams.outputCSV; }
        std::vector<std::string> GetPolyEvalFunctionVariants() const { return polyEvalParams.functionVariants; }
        std::vector<double> GetChebyshevCoefficients() const { return polyEvalParams.chebyshevCoeff; }

        // Accessors for file paths
        std::string GetClientContextFileLocation() const { return paths.contextClientFileLocation; }
        std::string GetServerContextFileLocation() const { return paths.contextServerFileLocation; }
        std::string GetSecretKeyFileLocation() const { return paths.secretKeyFileLocation; }
        std::string GetPublicKeyFileLocation() const { return paths.publicKeyFileLocation; }
        std::string GetRotKeysFileLocation() const { return paths.rotKeysFileLocation; }
        std::string GetMulKeysFileLocation() const { return paths.mulKeysFileLocation; }
        std::string GetKeysPath() const { return paths.keysPath; }
};

#endif // LOADER_H
