#include "../src/poly_eval.h"
#include "utils/config_loader.h"

void RunPolyEvalDemo() {
    ConfigLoader config("config.yaml");

    RunPolynomialEvaluation(config.GetPolyEvalFunctionVariants().front(), config.GetPolyEvalRingDims().front(), config.GetPolyEvalVectorSizes().front(), config.GetPolyEvalMaxVectorValue());

}
