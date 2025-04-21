#include "../src/matvec.h"
#include "utils/config_loader.h"

void RunMatvecDemo() {
    ConfigLoader config("config.yaml");
    auto matvecParams = config.get_matvec_params();

    RunMatrixVectorMultiplication(matvecParams.ringDims.front(), matvecParams.matrixSizes.front(), matvecParams.max_matrix_value, matvecParams.max_vector_value);

}
