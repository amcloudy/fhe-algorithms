#include "../src/matvec.h"
#include "../src/utils/config_loader.h"

void RunMatvecDemo() {
    ConfigLoader config("config.yaml");

    RunMatrixVectorMultiplication(config.GetMatVecFunctionVariants().front(), config.GetMatVecRingDims().front(), config.GetMatVecMatrixSizes().front(), config.GetMatVecMaxMatrixValue(), config.GetMatVecMaxVectorValue());

}
