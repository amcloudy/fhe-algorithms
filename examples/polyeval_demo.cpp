#include "../src/poly_eval.h"
#include "utils/config_loader.h"

void RunPolyEvalDemo() {
    ConfigLoader config("config.yaml");
    auto polyParams = config.get_polyeval_params();

    RunPolynomialEvaluation(polyParams.functionVariants.back(), polyParams.ringDims.front(), polyParams.vectorSizes.front(), polyParams.max_vector_value);

}
